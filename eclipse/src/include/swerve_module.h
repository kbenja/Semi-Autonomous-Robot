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
    uint16_t current_pos;     //swerve orientation
    uint16_t speed;         //swerve speed

    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t z_pos;

    bool limit_cw;          //rotational limits
    bool limit_ccw;

    bool correct_pos;
    bool rotating_ccw;
    bool rotating_cw;

    mraa_i2c_context i2c_context;   //i2c context for communication

    Motor_Module * steer_motor;         //steering motor
    Motor_Module * drive_motor;       //driving motor
    Pot_Module * dir_feedback;        //potentiometer for angular reference
    // Encoder_Module drive_feedback;  //optical encoder for drive



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
    Swerve_Module(const mraa_i2c_context & i2c_in, int module_id, int steer_port, int drive_port, int pot_adc, int encoder_port,
                    uint16_t x_position, uint16_t y_position, uint16_t z_position) {
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


        //printf("[ init ] Swerve module initialized with ID %d\n", module_id);
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
        Rotates swerve module to correct position for XYZ translation

        @param  char    position:       desired translation position (XYZ), case-insensitive

        @returns:   the result of the Y-translation positioning as a mraa_result_t value
    */
    int rotate_position(char position) {
        mraa_result_t result = MRAA_SUCCESS;
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
            return MRAA_ERROR_INVALID_PARAMETER;
        }

        current_pos = dir_feedback->get_average_val();    //get starting position
        if (desired_pos + 3 <= current_pos) {              //rotate CCW
            correct_pos = false;
            rotating_cw = false;
            if (!rotating_ccw) {
                result = rotate_ccw();
                if(result != -1) {
                    rotating_ccw = true;
                }
            }
        }
        else if (desired_pos - 3 >= current_pos) {         //rotate CW
            rotating_ccw = false;
            correct_pos = false;
            if (!rotating_cw) {
                result = rotate_cw();
                if(result != -1) {
                    rotating_cw = true;
                }
            }
        }
        else {
            rotating_cw = false;
            rotating_ccw = false;
            if(!correct_pos) {
                result = steer_motor->send_signal(i2c_context, 0); // stop steering
                if(result != -1) {
                    correct_pos = true;
                }
            }
        }
        if (result != MRAA_SUCCESS) {
            return -1;
        } else {
            return (current_pos ? 0 : 1); // if correct position send (0 = working) else (1 = still rotating)
        }
    }

    /*
        Rotates drive wheel forwards
    */
    int drive_wheel(float speed) {
        mraa_result_t result = drive_motor->send_signal(i2c_context, speed);
        if (result != MRAA_SUCCESS) {
            return -1;
        } else {
            return 0;
        }
    }

    /*
        Rotates wheel clockwise. Stops if limit is reached.
    */
    int rotate_cw() {
        mraa_result_t result = steer_motor->send_signal(i2c_context, -0.45);
        //printf("CW – Value: %d, Limit: %d\n", dir_feedback->get_val(), CW_LIMIT);
        if (result != MRAA_SUCCESS) {
            return -1;
        } else {
            return 0;
        }

    }

    /*
        Rotates wheel counter-clockwise. Stops if limit is reached.
    */
    int rotate_ccw() {
        mraa_result_t result = MRAA_SUCCESS;
        //printf("CCW – Value: %d, Limit: %d\n", dir_feedback->get_val(), CCW_LIMIT);
        result = steer_motor->send_signal(i2c_context, 0.45);
        if (result != MRAA_SUCCESS) {
            return -1;
        } else {
            return 0;
        }
    }

    /*
        Rotates drive wheel backwards
    */

    /*
        Stops all motors
    */
    mraa_result_t stop_motors() {
        printf("Stopping all motors\n");
        mraa_result_t result = steer_motor->send_signal(i2c_context, 0);
        result = drive_motor->send_signal(i2c_context, 0);
        return result;
    }


    /*
        Original working rotate function
    */
    mraa_result_t rotate(uint16_t desired_pos) {
        mraa_result_t result = MRAA_SUCCESS;
        uint16_t current_pos = dir_feedback->get_average_val();  //initial starting position
        if ((desired_pos - 3 >= current_pos) && (desired_pos < CW_LIMIT)) { //1926-2 = 1924
            result = rotate_cw();     //rotate clockwise to position -> increases value
        }
        else if ((desired_pos + 3 <= current_pos) && (desired_pos > CCW_LIMIT)) { //1926+2 = 1928
            result = rotate_ccw();    //rotate counterclockwise to position -> decreases value
        }
        else {
            result = steer_motor->send_signal(i2c_context, 0); //Stop Signal to Motor
            sleep(2);
        }
        return result;
    }
};

#endif

