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

    bool correct_pos;
    bool rotating_ccw;
    bool rotating_cw;

    bool ready;

    mraa_i2c_context i2c_context;   //i2c context for communication

    Motor_Module * steer_motor;         //steering motor
    Motor_Module * drive_motor;       //driving motor
    Pot_Module * dir_feedback;        //potentiometer for angular reference
    // Encoder_Module drive_feedback;  //optical encoder for drive

    int controller_result;

    /*
        Initializes swerve module with ID 0 and:
            2x Motor_Modules both on port 0
            1x Pot_Module on ADC 0
            1x Encoder_Module on port 0
    */
    Swerve_Module() {
        printf("[ !!!! ] Do not use default constructor\n");
    }

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
        id = module_id;
        steer_motor = new Motor_Module(steer_port);
        drive_motor = new Motor_Module(drive_port);
        dir_feedback = new Pot_Module(pot_adc, 12);
        // drive_feedback = new Encoder_Module(encoder_port);

        //potentiometer initialization
        current_pos = dir_feedback->get_val();
        limit_cw = (current_pos < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (current_pos > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        //encoder initialization
        speed = 0;

        //i2c initialization
        i2c_context = i2c_in;

        //position initialization
        x_pos = x_position;
        y_pos = y_position;
        z_pos = z_position;

        // correct position = false to start
        correct_pos = false;
        controller_result = 1;

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
    int swerve_controller(char axis, float speed, bool proceed) {
        if (speed != 0) { // if not braking
            controller_result = rotate_position(axis);
            if (controller_result == 1) {
                return 1; // still rotating
                ready = false;
            } else if (controller_result == 0 && !proceed) {
                ready = true;
                return 0; // ready but other motors are not ready
            } else if (controller_result == 0 && proceed) {
                controller_result = drive_wheel(speed);
                ready = true;
                return 0; // all motors ready to drive
            } else if (controller_result == -1) {
                ready = false;
                return -1;
            }
        } else {
            controller_result = stop_motors();
            return controller_result;
        }
        return 0; // okay;
    }

    /*
        Rotates swerve module to correct position for XYZ translation
        @param  char    position:       desired translation position (XYZ), case-insensitive
        @returns:   the result of the Y-translation positioning as a mraa_result_t value
    */
    int rotate_position(char position) {
        int rotation_result;
        uint16_t desired_pos;
        switch(position) {          //desired swerve position based on desired axis translation
        case 'X':
        case 'x':
            desired_pos = x_pos;
            break;
        case 'Y':
        case 'y':
            desired_pos = y_pos;
            break;
        case 'Z':
        case 'z':
            desired_pos = z_pos;
            break;
        default:
            return -1;
        }
        current_pos = dir_feedback->get_average_val();    //get starting position
        if (desired_pos + 5 <= current_pos) {              //rotate CCW
            correct_pos = false;
            rotating_cw = false;
            if (!rotating_ccw) {
                rotation_result = rotate_ccw();
                if(rotation_result != -1) {
                    rotating_ccw = true;
                }
            }
        } else if (desired_pos - 5 >= current_pos) {         //rotate CW
            rotating_ccw = false;
            correct_pos = false;
            if (!rotating_cw) {
                rotation_result = rotate_cw();
                if(rotation_result != -1) {
                    rotating_cw = true;
                }
            }
        } else {
            rotating_cw = false;
            rotating_ccw = false;
            if(!correct_pos) {
                rotation_result = stop_rotation(); // stop rotation
                sleep(1);
                if(rotation_result != -1) {
                    correct_pos = true;
                }
            }
        }
        if (rotation_result == -1) {
            return -1;
        } else {
            return (current_pos ? 0 : 1); // if correct position send (0 = working) else (1 = still rotating)
        }
    }

    /*
        Rotates drive wheel forwards
    */
    int drive_wheel(float speed) {
        printf("DRIVE module %d\n", id);
        mraa_result_t result = drive_motor->send_signal(i2c_context, speed);
        return (result != MRAA_SUCCESS ? -1 : 0);
    }

    /*
        Rotates wheel clockwise. Stops if limit is reached.
    */
    int rotate_cw() {
        printf("ROTATE CW module %d\n", id);
        mraa_result_t result = steer_motor->send_signal(i2c_context, -0.45);
        return (result != MRAA_SUCCESS ? -1 : 0);

    }

    /*
        Rotates wheel counter-clockwise. Stops if limit is reached.
    */
    int rotate_ccw() {
        mraa_result_t result = MRAA_SUCCESS;
        printf("ROTATE CCW module %d\n", id);
        result = steer_motor->send_signal(i2c_context, 0.45);
        return (result != MRAA_SUCCESS ? -1 : 0);
    }

    /*
        Stop rotating
    */
    int stop_rotation() {
        printf("STOP ROTATE on module %d\n", id);
        mraa_result_t result = steer_motor->send_signal(i2c_context, 0);
        return (result != MRAA_SUCCESS ? -1 : 0);
    }
    /*
        Stops all motors
    */
    int stop_motors() {
        printf("STOP ALL motors on module %d\n", id);
        mraa_result_t result = steer_motor->send_signal(i2c_context, 0);
        result = drive_motor->send_signal(i2c_context, 0);
        return (result != MRAA_SUCCESS ? -1 : 0);
    }


    /*
        Original working rotate function
    */
    // mraa_result_t rotate(uint16_t desired_pos) {
    //     mraa_result_t result = MRAA_SUCCESS;
    //     uint16_t current_pos = dir_feedback->get_average_val();  //initial starting position
    //     if ((desired_pos - 3 >= current_pos) && (desired_pos < CW_LIMIT)) { //1926-2 = 1924
    //         result = rotate_cw();     //rotate clockwise to position -> increases value
    //     }
    //     else if ((desired_pos + 3 <= current_pos) && (desired_pos > CCW_LIMIT)) { //1926+2 = 1928
    //         result = rotate_ccw();    //rotate counterclockwise to position -> decreases value
    //     }
    //     else {
    //         result = steer_motor->send_signal(i2c_context, 0); //Stop Signal to Motor
    //         sleep(2);
    //     }
    //     return result;
    // }
};

#endif

