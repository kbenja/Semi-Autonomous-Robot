#include "mraa.h"
#include "motor_module.h"
#include "i2c_library.h"

#ifndef SWERVE_MODULE_H
#define SWERVE_MODULE_H

class Swerve_Module {
    
    
    //internal values
    int id;                 //module unique ID
    uint16_t direction;     //swerve orientation
    uint16_t speed;         //swerve speed

    motor_module dir_motor;         //steering motor
    motor_module drive_motor;       //driving motor

    encoder_module 

public:
    //constructors
    /*
        Initializes swerve module with ID 0 and two Motor_Modules both on port 0
    */
    Swerve_Module() {
        id = 0;
        dir_motor = Motor_Module(0);
        drive_motor = Motor_Module(0);
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
        printf("[ init ] Swerve module initialized with ID %d\n", module_id);

    }

    /*
        Initializes swerve module with two Motor_Modules on specified ports
        @param int module_id:   ID of the swerve module
        @param dir_port:        ptr to motor controlling swerve direction
        @param drive_port:      ptr to motor controlling swerve movement

    */
    Swerve_Module(int module_id, int dir_port, int drive_port) {
        id = module_id;
        dir_motor = Motor_Module(dir_port);
        drive_motor = Motor_Module(drive_port);
        printf("[ init ] Swerve module initialized with ID %d\n", module_id);
    }

    //destructor
    ~Swerve_Module() {
        printf("[ dest ] Swerve module %d deleted\n", id);
    }

    //functions
    mraa_result_t rotate()


};

#endif