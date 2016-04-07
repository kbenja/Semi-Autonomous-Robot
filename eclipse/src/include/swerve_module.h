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
    int id;                 //module unique ID
    uint16_t direction;     //swerve orientation
    uint16_t speed;         //swerve speed

    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t z_pos;

    bool limit_cw;          //rotational limits
    bool limit_ccw;

    mraa_i2c_context i2c_context;   //i2c context for communication

    Motor_Module * steer_motor;         //steering motor
    Motor_Module * drive_motor;       //driving motor
    Pot_Module * dir_feedback;        //potentiometer for angular reference
    // Encoder_Module drive_feedback;  //optical encoder for drive

public:

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
        Initializes swerve module with specified ID and:
            2x Motor_Modules with specified ports
            1x Pot_Module on specified ADC
            1x Encoder_Module on specified port

        @param int module_id:   ID of the swerve module
        @param steer_port:        ptr to motor controlling swerve direction
        @param drive_port:      ptr to motor controlling swerve movement

    */
    Swerve_Module(const mraa_i2c_context & i2c_in, int module_id, int steer_port, int drive_port, int pot_adc, int encoder_port,
                    uint16_t x_position, uint16_t y_position, uint16_t z_position) {
        id = module_id;
        steer_motor = new Motor_Module(steer_port);
        drive_motor = new Motor_Module(drive_port);
        dir_feedback = new Pot_Module(pot_adc, 12);
        // drive_feedback = new Encoder_Module(encoder_port);

        //potentiometer initialization
        direction = dir_feedback->get_val();
        limit_cw = (direction < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (direction > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        //encoder initialization
        speed = 0;

        //i2c initialization
        i2c_context = i2c_in;

        //position initialization
        x_pos = x_position;
        y_pos = y_position;
        z_pos = z_position;


        printf("[ init ] Swerve module initialized with ID %d\n", module_id);
    }

    /*
        Default destructor for swerve module.
    */
    ~Swerve_Module() {
        printf("[ dest ] Swerve module %d deleted\n", id);
    }

    /*
        Rotates swerve module to correct position for Y translation. Position given as a @param in
            constructor of Swerve_Module().

        @returns:   the result of the Y-translation positioning as a mraa_result_t value
    */
    mraa_result_t rotate_position(char position) {
        mraa_result_t result = MRAA_SUCCESS;
        uint16_t desired_pos;
        switch(position) {
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
        direction = dir_feedback->get_average_val();    //get starting position
        if (desired_pos - 3 > direction) {              //rotate CCW
            result = rotate_ccw();
        }
        else if (desired_pos - 3 < direction) {         //rotate CW
            result = rotate_cw();
        }
        else {                                          //close enough; stop steering
            result = steer_motor->send_signal(i2c_context, 0);
            sleep(2);
        }
        return result;
    }








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

    /*
        Rotates wheel clockwise. Stops if limit is reached.
    */
    mraa_result_t rotate_cw() {
        mraa_result_t result = steer_motor->send_signal(i2c_context, -0.35);
        printf("CW – Value: %d, Limit: %d\n", dir_feedback->get_val(), CW_LIMIT);
        return result;
        /*
        limit_ccw = false;
        if (dir_feedback->get_val() >= CW_LIMIT) { //if limit reached
            limit_cw = true;//stop
        }
          */
    }

    /*
        Rotates wheel counter-clockwise. Stops if limit is reached.
    */
    mraa_result_t rotate_ccw() {
        mraa_result_t result = MRAA_SUCCESS;
        printf("CCW – Value: %d, Limit: %d\n", dir_feedback->get_val(), CCW_LIMIT);
        result = steer_motor->send_signal(i2c_context, 0.35);
        return result;
    }

    /*
        Rotates drive wheel forwards
    */
    mraa_result_t drive_forward(float speed) {
        mraa_result_t result = drive_motor->send_signal(i2c_context, speed);
        printf("Driving wheel FORWARDS");
        return result;
    }

    /*
        Rotates drive wheel backwards
    */
    mraa_result_t drive_backward(float speed) {
        mraa_result_t result  = drive_motor->send_signal(i2c_context, speed);
        printf("Driving wheel BACKWARDS");
        return result;
    }

    /*
        Stops all motors
    */
    mraa_result_t stop_motors() {
        printf("Stopping all motors\n");
        mraa_result_t result = steer_motor->send_signal(i2c_context, 0);
        result = drive_motor->send_signal(i2c_context, 0);
        return result;
    }


};

#endif

