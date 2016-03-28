#include "mraa.h"
#include "motor_module.h"
#include "i2c_library.h"

#ifndef SWERVE_MODULE_H
#define SWERVE_MODULE_H

class Swerve_Module {
    //external objects
    motor_module* dir_motor;        //pointer to steering motor
    motor_module* drive_motor;      //pointer to driving motor
    //internal values
    int id;     //module unique ID
    uint16_t direction;     //swerve orientation
    uint16_t speed;         //swerve speed

public:

    /*
        Initializes swerve module with default ID 0 and NULL ptrs to
            motor_modules
    */
    Swerve_Module() {
        id = 0;
        dir_motor = NULL;
        drive_motor = NULL;
        printf("[ init ] Swerve module initialized with ID 0\n");
    }

    /*
        Initializes swerve module with NULL ptrs to
            motor_modules
        @param int module_id:   ID of the swerve module
    */
    Swerve_Module(int module_id) {
        id = module_id;
        dir_motor = NULL;
        drive_motor = NULL;
        printf("[ init ] Swerve module initialized with ID %d\n", module_id);
    }

    /*
        Initializes swerve module with NULL ptrs to
            motor_modules
        @param int module_id:               ID of the swerve module
        @param motor_module* direction:     ptr to motor controlling swerve direction
        @param motor_module* drive:         ptr to motor controlling swerve movement

    */
    Swerve_Module(int module_id, motor_module* direction, motor_module* drive) {
        id = module_id;
        dir_motor = direction;
        drive_motor = drive;
        printf("[ init ] Swerve module initialized with ID %d.\n", module_id);
        printf("         Rotation motor associated.\n");
        printf("         Drive motor associated.\n");
    }





};

#endif