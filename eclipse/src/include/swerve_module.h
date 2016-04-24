#include "mraa.h"
#include "motor_module.h"
#include "i2c_library.h"
#include "pot_module.h"

#define CW_LIMIT 2529   //clockwise limit
#define CCW_LIMIT 1505  //counter-clockwise limit
#define CENTER 2047     //ideal center

#ifndef SWERVE_MODULE_H
#define SWERVE_MODULE_H

class Swerve_Module {
public:
    int id;                 //module unique ID
    uint16_t current_pos;   //swerve orientation
    uint16_t speed;         //swerve speed

    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t z_pos;

    bool limit_cw;          //rotational limits
    bool limit_ccw;

    // swerve_controller state variables
    bool correct_pos; // used by drive module to know when each swerve module is in the correct position
    bool waiting; // true when swerve module is rotated to the correct position, but is waiting for proceed value
    bool has_passed; // used in rotate_position for overshoot logic
    bool is_rotating_ccw; // used in rotate_position to reduce signals sent to i2c chip
    bool is_rotating_cw; // used in rotate_position to reduce signals sent to i2c chip
    bool is_driving; // used in rotate_position to reduce signals sent to i2c chip
    char last_position; // used to store last direction state to reduce amount of calibration needed

    // stopping state variables
    bool is_stopping;   // use to reduce signals sent to i2c chip
    bool saved_last_position;

    mraa_i2c_context i2c_context;   //i2c context for communication

    Motor_Module * steer_motor;         //steering motor
    Motor_Module * drive_motor;       //is_driving motor
    Pot_Module * dir_feedback;        //potentiometer for angular reference
    // Encoder_Module drive_feedback;  //optical encoder for drive

    int controller_result;

    /*
        Initializes swerve module with specified ID:

        @param const        mraa_i2c_context &i2c_in:   i2c context for communication
        @param int          module_id:                  ID of the swerve module
        @param int          steer_port:                 ptr to motor controlling swerve direction
        @param int          drive_port:                 ptr to motor controlling swerve movement
        @param int          pot_adc:                    potentiometer ADC port
        @param int          encoder_port:               encoder port/I2C address
        @param unint16_t    x_position:                 swerve position for x translation
        @param unint16_t    y_position:                 swerve position for y translation
        @param unint16_t    z_position:                 swerve position for z translation

        @returns:                                   Swerve_Module object

    */
    Swerve_Module(const mraa_i2c_context & i2c_in, int module_id, int steer_port, int drive_port, int pot_adc, int encoder_port, uint16_t x_position, uint16_t y_position, uint16_t z_position) {
        this->id = module_id;
        this->steer_motor = new Motor_Module(steer_port);
        this->drive_motor = new Motor_Module(drive_port);
        this->dir_feedback = new Pot_Module(pot_adc, 12);
        // drive_feedback = new Encoder_Module(encoder_port);

        //potentiometer initialization
        this->current_pos = dir_feedback->get_val();
        this->limit_cw = (current_pos < CW_LIMIT) ? false : true;    //check for clockwise limit
        this->limit_ccw = (current_pos > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        //encoder initialization
        speed = 0;

        //i2c initialization
        i2c_context = i2c_in;

        //position initialization
        this->x_pos = x_position;
        this->y_pos = y_position;
        this->z_pos = z_position;

        this->has_passed = false;
        this->waiting = false;
        this->is_driving = false;
        this->last_position = 'q';
        printf("[ init ] Created swerve module ID %d\n", id);
    }

    /*
        Default destructor for swerve module.
    */
    ~Swerve_Module() {  //free memory
        delete steer_motor;
        delete drive_motor;
        delete dir_feedback;
        //delete drive_feedback;
        printf("[ dest ] Swerve module %d deleted\n", id);
    }

    /*
        Decides when to rotate or drive the swerve module. Waits for Driving module's OK to continue
        @param  char    axis:       desired translation position (XYZ), case-insensitive
        @param  float   speed:      desired speed for the drive wheel to turn when ready
        @param  bool    proceed:    the OK signal received which means all 4 swerve modules are ready to continue
        @returns:       1 if still rotating direction motor, 0 if ready to move, -1 if error has occured
    */
    int swerve_controller(char axis, float speed, bool proceed, bool wait) {
        is_stopping = false;
        uint16_t desired_pos;
        switch(axis) {          //desired swerve position based on desired axis translation
            case 'X':
            case 'x':
                desired_pos = this->x_pos; // handle BL and BR motors when is_driving in X direction
                if (this->id == 1 || this->id == 3) {
                    speed *= -1;
                }
                break;
            case 'Y':
            case 'y':
                desired_pos = this->y_pos; // handle BR and FR motors when is_driving in Y direction
                if (this->id == 3 || this->id == 4) {
                    speed *= -1;
                }
                break;
            case 'Z':
            case 'z':
                desired_pos = this->z_pos;
                break;
            default:
                return -1;
        }
        controller_result = 0; // assume function returns OKAY from the start
        if (!wait) {
            this->waiting = false; // reset waiting boolean
            printf("AXIS: %c\n", axis);
            if(axis == last_position) { // save last position
                this->correct_pos =  true;
                return 0;
            }
            controller_result = rotate_position(axis, desired_pos);
            if (speed == 0) {
                controller_result = stop_motors();
                return controller_result;
            }
            return controller_result;
        } else {
            if (this->correct_pos) { // if the last calibration didn't finish calibrating
                this->last_position = axis;
            }
            if(!waiting) {
                this->waiting = true;
                printf("Wheel %d is in position.\n", id);
                stop_rotation();
            }
            if (proceed && !is_driving) { // received proceed command from drive module
                controller_result = drive_wheel(speed);
                printf("Driving wheel. %d\n", id);
                this->is_driving = true;
                return controller_result; // returns 0 if good, -1 if error
            } else {
                return 0; // returns
            }
        }
        return 0;
    }

    /*
        Rotates swerve module to correct position for XYZ translation
        @param  char    position:       desired translation position (XYZ), case-insensitive
        @returns:   the result of the Y-translation positioning as a mraa_result_t value
    */
    int rotate_position(char position, uint16_t desired_pos) {
        int rotation_result;
        current_pos = dir_feedback->get_average_val();    //get starting position
        // printf("current_pos: %d, desired_pos: %d\n", current_pos, desired_pos);
        // return 0;
        if (desired_pos + 40 <= current_pos) {
            // overshoot and then approach from CW side
            this->correct_pos = false;    //not in the correct position
            this->is_rotating_cw = false;    //not rotating clockwise
            this->has_passed = true;      //overshot position
            if (!is_rotating_ccw) {
                rotation_result = rotate_ccw(); // increase pot value
                if(rotation_result != -1) { // check for errors
                    is_rotating_ccw = true;
                }
            }
        } else if (desired_pos - 40 >= current_pos) {         //rotate CW
            this->correct_pos = false;
            this->is_rotating_ccw = false;
            this->has_passed = false;
            if (!is_rotating_cw) {
                rotation_result = rotate_cw(); // decrease pot value
                if(rotation_result != -1) { // check for errors
                    is_rotating_cw = true;
                }
            }
        } else if (has_passed) {
            this->is_rotating_cw = false;
            this->is_rotating_ccw = false;
            if(!correct_pos) {
                rotation_result = stop_rotation(); // stop rotation
                if(rotation_result != -1) {
                    correct_pos = true;
                }
            }
        }
        if (rotation_result == -1) {
            return -1;
        } else {
            return (correct_pos ? 0 : 1); // if correct position send (0 = correct position) else (1 = still rotating)
        }
    }

    /*
        Rotates drive wheel forwards
    */
    int drive_wheel(float speed) {
        mraa_result_t result = drive_motor->send_signal(i2c_context, speed);
        return (result != MRAA_SUCCESS ? -1 : 0);
    }

    /*
        Rotates wheel clockwise. Stops if limit is reached.
    */
    int rotate_cw() {
        mraa_result_t result = steer_motor->send_signal(i2c_context, -0.4);
        return (result != MRAA_SUCCESS ? -1 : 0);

    }

    /*
        Rotates wheel counter-clockwise. Stops if limit is reached.
    */
    int rotate_ccw() {
        mraa_result_t result = MRAA_SUCCESS;
        result = steer_motor->send_signal(i2c_context, 0.6);
        return (result != MRAA_SUCCESS ? -1 : 0);
    }

    /*
        Stop rotating
    */
    int stop_rotation() {
        mraa_result_t result = steer_motor->send_signal(i2c_context, 0);
        return (result != MRAA_SUCCESS ? -1 : 0);
    }
    /*
        Stops all motors
    */
    int stop_motors() {
        mraa_result_t result = steer_motor->send_signal(i2c_context, 0);
        // stop if module isn't already stopped
        if (!is_stopping) {
            result = drive_motor->send_signal(i2c_context, 0);
            printf("STOP ALL motors on module %d\n", id);
            if(result == MRAA_SUCCESS) {
                is_stopping = true;
            }
        }
        // reset all variables to false for next turn
        this->correct_pos = false;
        this->is_driving = false;
        this->is_rotating_ccw = false;
        this->is_rotating_cw = false;
        this->waiting = false;
        return (result != MRAA_SUCCESS ? -1 : 0);
    }
};

#endif

