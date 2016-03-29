#include "mraa.h"
#include "motor_module.h"
#include "i2c_library.h"


#define CW_LIMIT 2529   //clockwise limit
#define CCW_LIMIT 1505  //counter-clockwise limit
#define CENTER 2047     //ideal center

#ifndef SWERVE_MODULE_H
#define SWERVE_MODULE_H

class Swerve_Module {
    //internal values
    int id;                 //module unique ID
    uint16_t direction;     //swerve orientation
    uint16_t speed;         //swerve speed

    bool limit_cw;          //rotational limits
    bool limit_ccw;


    Motor_Module dir_motor;         //steering motor
    Motor_Module drive_motor;       //driving motor

    Pot_Module dir_feedback;        //potentiometer for angular reference
    Encoder_Module drive_feedback;  //optical encoder for drive

public:
    //constructors
    /*
        Initializes swerve module with ID 0 and:
            2x Motor_Modules both on port 0
            1x Pot_Module on ADC 0
            1x Encoder_Module on port 0
    */
    Swerve_Module() {
        id = 0;

        //object creation
        dir_motor = Motor_Module(0);
        drive_motor = Motor_Module(0);
        dir_feedback = Pot_Module(0);
        drive_feedback = Encoder_Module(0);

        //potentiometer initialization
        direction = dir_feedback.get_val();
        limit_cw = (direction < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (direction > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        //encoder initialization
        speed = 0;

        //i2c initialization
        i2c_context = mraa_i2c_init(6);
        mraa_i2c_address(i2c_context, 0x40);

        dir_motor = Motor_Module(0);
        drive_motor = Motor_Module(0);
        pot_module = Pot_Module(0);
        encoder_module = Encoder_Module(0);
        limit_cw = (pot_module.get_val() < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (pot_module.get_val() > CCW_LIMIT) ? false : true;  //check for c-clockwise limit
        dir_feedback = Pot_Module(0);
        drive_feedback = Encoder_Module(0);

        //potentiometer initialization
        direction = dir_feedback.get_val();
        limit_cw = (direction < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (direction > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        //encoder initialization
        speed = 0;
        printf("[ init ] Swerve module initialized with ID 0\n");
    }

    /*
        Initializes swerve module with ID 0 and:
            2x Motor_Modules both on port 0
            1x Pot_Module on ADC 0
            1x Encoder_Module on port 0

        @param int module_id:   ID of the swerve module
    */
    Swerve_Module(int module_id) {
        id = module_id;
        dir_motor = Motor_Module(0);
        drive_motor = Motor_Module(0);
        dir_feedback = Pot_Module(0);
        drive_feedback = Encoder_Module(0);

        //potentiometer initialization
        direction = dir_feedback.get_val();
        limit_cw = (direction < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (direction > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        //encoder initialization
        speed = 0;

        //i2c initialization
        i2c_context = mraa_i2c_init(6);
        mraa_i2c_address(i2c_context, 0x40);
        pot_module = Pot_Module(0);
        encoder_module = Encoder_Module(0);
        limit_cw = (pot_module.get_val() < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (pot_module.get_val() > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        printf("[ init ] Swerve module initialized with ID %d\n", module_id);

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
        dir_motor = Motor_Module(dir_port);
        drive_motor = Motor_Module(drive_port);
        dir_feedback = Pot_Module(pot_adc);
        drive_feedback = Encoder_Module(encoder_port);

        //potentiometer initialization
        direction = dir_feedback.get_val();
        limit_cw = (direction < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (direction > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        //encoder initialization
        speed = 0;

        //i2c initialization
        i2c_context = i2c_in;
        mraa_i2c_address(i2c_context, 0x40);

        printf("[ init ] Swerve module initialized with ID %d\n", module_id);
    }


    /*
        Default destructor for swerve module.

    */
    ~Swerve_Module() {
        printf("[ dest ] Swerve module %d deleted\n", id);
    }

    //functions

    /*
        @param uint16_t pos:    desired ADC value of rotation (2047 = middle)

        @returns:   the result of the operation as a mraa_result_t value
    */
    mraa_result_t rotate(uint16_t pos) {
        uint16_t current_pos = dir_feedback.get_val();  //initial starting position
        if ((pos > current_pos) && (pos < CW_LIMIT)) {
            return rotate_cw();     //rotate clockwise to position
        }
        else if ((pos < current_pos) && (pos > CCW_LIMIT)) {
            return rotate_ccw();    //rotate counterclockwise to position
        }
    }

    /*
        Rotates wheel clockwise. Stops if limit is reached.
    */
    mraa_result_t rotate_cw() {
        dir_motor.send_signal(i2c_context, -0.3);
        printf("CW – Value: %d, Limit: %d\n", dir_feedback.get_val(), CW_LIMIT);
        return 0;
        /*
        limit_ccw = false;
        if (dir_feedback.get_val() >= CW_LIMIT) { //if limit reached
            limit_cw = true;//stop
        }
          */
        //else
            //send signal to motor to rotate clockwise
    }

    /*
        Rotates wheel counter-clockwise. Stops if limit is reached.
    */
    mraa_result_t rotate_ccw() {
        dir_motor.send_signal(i2c_context, 0.3);
        printf("CCW – Value: %d, Limit: %d\n", dir_feedback.get_val(), CCW_LIMIT);
        return 0;
        //if limit not reached
    }

    /*
        Rotates drive wheel forwards
    */
    mraa_result_t drive_forward(float speed) {
        dir_motor.send_signal(i2c_context, speed);
        printf("Driving wheel FORWARDS");
        return 0;
    }

    /*
        Rotates drive wheel backwards
    */
    mraa_result_t drive_backward(float speed) {
        dir_motor.send_signal(i2c_context, speed);
        printf("Driving wheel BACKWARDS");
        return 0;
    }

    /*
        Stops all motors
    */
    mraa_result_t stop_motors() {
        printf("Stopping all motors\n");
        dir_motor.send_signal(i2c_context, 0);
        drive_motor.send_signal(i2c_context, 0);
    }


};

#endif

