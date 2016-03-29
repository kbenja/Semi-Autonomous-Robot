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

    motor_module dir_motor;         //steering motor
    motor_module drive_motor;       //driving motor

    pot_module dir_feedback;        //potentiometer for angular reference
    encoder_module drive_feedback;  //optical encoder for drive

    pot_module dir_feedback;        //potentiometer for angular reference
    encoder_module drive_feedback;  //optical encoder for drive


public:
    //constructors
    /*
        Initializes swerve module with ID 0 and two Motor_Modules both on port 0
    */
    Swerve_Module() {
        id = 0;
        dir_motor = Motor_Module(0);
        drive_motor = Motor_Module(0);

        pot_module = Pot_Module(0);
        encoder_module = Encoder_Module(0);
        limit_cw = (pot_module.get_val() < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (pot_module.get_val() > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        printf("[ init ] Swerve module initialized with ID 0\n");
    }

    /*
        Initializes swerve module with two Motor_Modules both on port 0
        @param int module_id:   ID of the swerve module
    */
    Swerve_Module(int module_id) {
        id = module_id;
        dir_motor = Motor_Module(0);
        drive_motor = Motor_Module(0);

        pot_module = Pot_Module(0);
        encoder_module = Encoder_Module(0);
        limit_cw = (pot_module.get_val() < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (pot_module.get_val() > CCW_LIMIT) ? false : true;  //check for c-clockwise limit

        printf("[ init ] Swerve module initialized with ID %d\n", module_id);

    }

    /*
        Initializes swerve module with two Motor_Modules on specified ports

        @param int module_id:   ID of the swerve module
        @param dir_port:        ptr to motor controlling swerve direction
        @param drive_port:      ptr to motor controlling swerve movement

    */
    Swerve_Module(int module_id, int dir_port, int drive_port, int pot_adc, int encoder_port) {
        id = module_id;
        dir_motor = Motor_Module(dir_port);
        drive_motor = Motor_Module(drive_port);
        pot_module = Pot_Module(pot_adc);
        encoder_module = Encoder_Module(encoder_port);
        limit_cw = (pot_module.get_val() < CW_LIMIT) ? false : true;    //check for clockwise limit
        limit_ccw = (pot_module.get_val() > CCW_LIMIT) ? false : true;  //check for c-clockwise limit
        printf("[ init ] Swerve module initialized with ID %d\n", module_id);
    }

    //destructor
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
        @param int degree: desired degree of rotation (0 = start)

        @returns:   the result of the operation as a mraa_result_t value
    */


    /*
        Rotates wheel clockwise. Stops if limit is reached.
    */
    mraa_result_t rotate_cw() {

        limit_ccw = false;
        if (pot_module.get_val() >= CW_LIMIT) { //if limit reached
            limit_cw = true;//stop
        }

        //else
            //send signal to motor to rotate clockwise

    }


    /*
        Rotates wheel counter-clockwise. Stops if limit is reached.
    */
    mraa_result_t rotate_ccw() {
        //if limit not reached
    }

};

#endif
