#include <stdlib.h>
#include "mraa.h"
#include "swerve_module.h"

#ifndef DRIVE_MODULE_H
#define DRIVE_MODULE_H

class Drive_Module {
public:
    mraa_i2c_context i2c_bus;
    bool FR_ready;
    bool BR_ready;
    bool FL_ready;
    bool BL_ready;
    bool drive_proceed; // waits for all 4 swerve modules to be ready
    int swerve_controller_result;

    Swerve_Module * FR;
    Swerve_Module * BR;
    Swerve_Module * FL;
    Swerve_Module * BL;

    Drive_Module(mraa_i2c_context i2c) {
    	i2c_bus = i2c;
        //initialize Swerve_Modules (ID, dir port, drive port, AOIN, Encoder, X, Y, Z)

        FR = new Swerve_Module(i2c, 4, 6, 7, 0, 0, 1491, 985, 2186); // WHEEL 0
        BR = new Swerve_Module(i2c, 3, 4, 5, 1, 0, 1463, 968, 1812); // WHEEL 1
        BL = new Swerve_Module(i2c, 1, 0, 1, 2, 0, 2510, 2030, 2161); // WHEEL 2
        FL = new Swerve_Module(i2c, 2, 2, 3, 3, 0, 2775, 2310, 2103); // WHEEL 3

        //initialize proceed flags
        FR_ready = false;
        BR_ready = false;
        FL_ready = false;
        BL_ready = false;
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
        FR_ready = (FR->correct_pos == 1 ? true : false);
        FL_ready = (FL->correct_pos == 1 ? true : false);
        BR_ready = (BR->correct_pos == 1 ? true : false);
        BL_ready = (BL->correct_pos == 1 ? true : false);
        if(FR_ready && FL_ready && BR_ready && BL_ready) {
            drive_proceed = true;
        } else {
            drive_proceed = false;
        }
        swerve_controller_result = FR->swerve_controller(axes, speed, drive_proceed, FR_ready);
        swerve_controller_result = FL->swerve_controller(axes, speed, drive_proceed, FL_ready);
        swerve_controller_result = BR->swerve_controller(axes, speed, drive_proceed, BR_ready);
        swerve_controller_result = BL->swerve_controller(axes, speed, drive_proceed, BL_ready);
        if (drive_proceed == true) {
            return 2;
        }
        return swerve_controller_result;
    }

    int stop() {
        swerve_controller_result = FR->stop_motors();
        swerve_controller_result = FL->stop_motors();
        swerve_controller_result = BR->stop_motors();
        swerve_controller_result = BL->stop_motors();
        drive_proceed = false;
        return swerve_controller_result;
    }

    void clear_cached() {
        FR->clear_cached();
        FL->clear_cached();
        BR->clear_cached();
        BL->clear_cached();
    }

};

#endif
