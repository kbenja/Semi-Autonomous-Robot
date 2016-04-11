#include <stdlib.h>
#include "mraa.h"
#include "swerve_module.h"

#ifndef DRIVE_MODULE_H
#define DRIVE_MODULE_H

class Drive_Module {
public:
    mraa_i2c_context i2c_bus;
    bool FR_swerve_proceed;
    bool BR_swerve_proceed;
    bool FL_swerve_proceed;
    bool BL_swerve_proceed;
    bool drive_proceed; // waits for all 4 swerve modules to be ready
    int swerve_controller_result;

    Swerve_Module * FR;
    Swerve_Module * BR;
    Swerve_Module * FL;
    Swerve_Module * BL;

    /*
      Default constructor for Swerve_Module. DO NOT CALL
    */
    Drive_Module() {
        printf("[ !!!! ] Do not use default constructor of steering module\n");
    }

    Drive_Module(mraa_i2c_context i2c) {
    	i2c_bus = i2c;
        //initialize Swerve_Modules
        FR = new Swerve_Module(i2c, 1, 1, 5, 1, 0, 2451, 1952, 2087);
        BR  = new Swerve_Module(i2c, 2, 2, 6, 2, 0, 1392, 1877, 1733);
        FL	= new Swerve_Module(i2c, 3, 3, 7, 3, 0, 1533, 2002, 1725);
        BL   = new Swerve_Module(i2c, 4, 4, 8, 4, 0, 2488, 1994, 2139);

        //initialize proceed flags
        FR_swerve_proceed = false;
        BR_swerve_proceed = false;
        FL_swerve_proceed = false;
        BR_swerve_proceed = false;
        drive_proceed = false;
        swerve_controller_result = 1;
    }

    ~Drive_Module() {   //free memory
        delete FR;
        delete BR;
        delete FL;
        delete BL;
    }

    int drive(char axes, float speed){
        FR_swerve_proceed = FR->ready;
        FL_swerve_proceed = FL->ready;
        BR_swerve_proceed = BR->ready;
        BL_swerve_proceed = BL->ready;
        if(FR_swerve_proceed || FL_swerve_proceed || BR_swerve_proceed || BL_swerve_proceed) {
            drive_proceed = true;
            return 0;
        } else {
            drive_proceed = false;
            swerve_controller_result = FR->swerve_controller(axes, speed, drive_proceed);
            if(swerve_controller_result == -1) {
                printf("FRONT RIGHT WHEEL STILL TURNING, AXIS = %c\n", axes);
            }
            swerve_controller_result = FL->swerve_controller(axes, speed, drive_proceed);
            if(swerve_controller_result == -1) {
                printf("FRONT LEFT WHEEL STILL TURNING, AXIS = %c\n", axes);
            }
            swerve_controller_result = BR->swerve_controller(axes, speed, drive_proceed);
            if(swerve_controller_result == -1) {
                printf("BACK RIGHT WHEEL STILL TURNING, AXIS = %c\n", axes);
            }
            swerve_controller_result = BL->swerve_controller(axes, speed, drive_proceed);
            if(swerve_controller_result == -1) {
                printf("BACK LEFT WHEEL STILL TURNING, AXIS = %c\n", axes);
            }
            return 1;
        }
    }
};

#endif
