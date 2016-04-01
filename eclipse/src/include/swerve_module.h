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

    bool limit_cw;          //rotational limits
    bool limit_ccw;

    mraa_i2c_context i2c_context;   //i2c context for communication

    Motor_Module * dir_motor;         //steering motor
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
        printf("[ !!!! ] Do not use defualt constructor\n");
    }

    /*
        Initializes swerve module with specified ID and:
            2x Motor_Modules with specified ports
            1x Pot_Module on specified ADC
            1x Encoder_Module on specified port

        @param int module_id:   ID of the swerve module
        @param dir_port:        ptr to motor controlling swerve direction
        @param drive_port:      ptr to motor controlling swerve movement

    */
    Swerve_Module(const mraa_i2c_context & i2c_in, int module_id, int dir_port, int drive_port, int pot_adc, int encoder_port) {
        id = module_id;
        dir_motor = new Motor_Module(dir_port);
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

        printf("[ init ] Swerve module initialized with ID %d\n", module_id);
    }

    /*
        Default destructor for swerve module.
    */
    ~Swerve_Module() {
        printf("[ dest ] Swerve module %d deleted\n", id);
    }

    /*
        @param uint16_t pos:    desired ADC value of rotation (2047 = middle)

        @returns:   the result of the operation as a mraa_result_t value
    */
    mraa_result_t rotate(uint16_t pos) {
        mraa_result_t result = MRAA_SUCCESS;
        uint16_t current_pos = dir_feedback->get_average_val();  //initial starting position
        if ((pos - 2 > current_pos || current_pos ) && (pos < CW_LIMIT)) {
            result = rotate_cw();     //rotate clockwise to position -> increases value
        }
        else if ((pos + 2 < current_pos) && (pos > CCW_LIMIT)) {
            result = rotate_ccw();    //rotate counterclockwise to position -> decreases value
        } else {
            result = dir_motor->send_signal(i2c_context, 0);
            sleep(1);
        }
        return result;
    }

    /*
        Rotates wheel clockwise. Stops if limit is reached.
    */
    mraa_result_t rotate_cw() {
        mraa_result_t result = dir_motor->send_signal(i2c_context, -0.35);
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
        result = dir_motor->send_signal(i2c_context, 0.35);
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
        mraa_result_t result = dir_motor->send_signal(i2c_context, 0);
        result = drive_motor->send_signal(i2c_context, 0);
        return result;
    }


};

#endif
