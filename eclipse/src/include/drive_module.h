#include <stdlib.h>
#include "mraa.h"
#include "swerve_module.h"

#ifndef DRIVE_MODULE_H
#define DRIVE_MODULE_H

class Drive_Module {
public:
    int id;
    mraa_i2c_context i2c_bus;

    Swerve_Module * front_left
    Swerve_Module * front_right;
    Swerve_Module * back_left;
    Swerve_Module * back_right;

    /*
      Default constructor for Swerve_Module. DO NOT CALL
    */
    Drive_Module() {
        printf("[ !!!! ] Do not use default constructor of steering module\n");
    }

    Drive_Module(int id) {
        this->id = id;

        //initialize Swerve_Modules
        front_left  = new Swerve_Module(i2c_bus, 1, 0, 4, 0, 0, 1024, 2048, 3072);
        front_right = new Swerve_Module(i2c_bus, 2, 1, 5, 1, 1, 1024, 2048, 3072);
        back_left   = new Swerve_Module(i2c_bus, 3, 2, 6, 2, 2, 1024, 2048, 3072);
        back_right  = new Swerve_Module(i2c_bus, 4, 3, 7, 3, 3, 1024, 2048, 3072);
    }

    ~Drive_Module() {   //free memory
        delete front_left;
        delete front_right;
        delete back_left;
        delete back_right;
    }



};

#endif
