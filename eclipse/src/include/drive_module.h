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

        // recorded 2 days after calibrated
        FR = new Swerve_Module(i2c, 4, 6, 7, 0, 0, 2437, 1925, 2075); // FR swerve S4
        BR = new Swerve_Module(i2c, 3, 4, 5, 1, 0, 1484, 1952, 1828); // BR swerve S3
        BL = new Swerve_Module(i2c, 1, 0, 1, 2, 0, 2506, 2006, 2173); // BL swerve S1
        FL = new Swerve_Module(i2c, 2, 2, 3, 3, 0, 1662, 2175, 1981); // FL swerve S2

        // // recorded 4/13 about 1 week after caibration
        // FR = new Swerve_Module(i2c, 4, 6, 7, 0, 0, 2860, 2282, 2443); // FR swerve S4
        // BR = new Swerve_Module(i2c, 3, 4, 5, 1, 0, 1635, 2200, 2062); // BR swerve S3
        // BL = new Swerve_Module(i2c, 1, 0, 1, 2, 0, 2881, 2290, 2442); // BL swerve S1
        // FL = new Swerve_Module(i2c, 2, 2, 3, 3, 0, 1974, 2542, 2362); // FL swerve S2

        // FR = new Swerve_Module(i2c, 4, 6, 7, 0, 0, 2490, 2124, 1981); // FR swerve S4
        // BR = new Swerve_Module(i2c, 3, 4, 5, 1, 0, 1727, 2224, 2083); // BR swerve S3
        // BL = new Swerve_Module(i2c, 1, 0, 1, 2, 0, 2881, 2290, 2442); // BL swerve S1
        // FL = new Swerve_Module(i2c, 2, 2, 3, 3, 0, 1974, 2542, 2362); // FL swerve S2

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
        FR_ready = FR->ready;
        FL_ready = FL->ready;
        BR_ready = BR->ready;
        BL_ready = BL->ready;
        if(FR_ready && FL_ready && BR_ready && BL_ready) {
            drive_proceed = true;
            printf("Started to move!\n");
        } else {
            drive_proceed = false;
        }
        swerve_controller_result = FR->swerve_controller(axes, speed, drive_proceed, FR_ready);
        swerve_controller_result = FL->swerve_controller(axes, speed, drive_proceed, FL_ready);
        swerve_controller_result = BR->swerve_controller(axes, speed, drive_proceed, BR_ready);
        swerve_controller_result = BL->swerve_controller(axes, speed, drive_proceed, BL_ready);
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
};

#endif
