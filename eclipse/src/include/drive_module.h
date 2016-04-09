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
    int drive_proceed;
    int swerve_controller_proceed;

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
        drive_proceed = 1;
        swerve_controller_proceed = 1;

    }

    ~Drive_Module() {   //free memory

        delete FR;
        delete BR;
        delete FL;
        delete BL;
    }

    int drive(char axes, float speed){
    	switch(axes) {          //desired swerve position based on desired axis translation
    		case 'X':
    	    case 'x':
    	    	if (!(FR_swerve_proceed || BR_swerve_proceed || FL_swerve_proceed || BL_swerve_proceed))
    	    	{
    	    		swerve_controller_proceed = FR.swerve_controller('X', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			FR_swerve_proceed = true;
    	    		}
    	    		swerve_controller_proceed = BR.swerve_controller('X', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			BR_swerve_proceed = true;
    	    		}
    	    		swerve_controller_proceed = FL.swerve_controller('X', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			FL_swerve_proceed = true;
    	    		}
    	    		swerve_controller_proceed = BL.swerve_controller('X', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			BL_swerve_proceed = true;
    	    		}
    	    	}
    	    	else if (!FR_swerve_proceed)
    	    	{
    	    		printf("FRONT RIGHT WHEEL STILL TURNING");
    	    	}
    	    	else if (!BR_swerve_proceed)
    	    	{
    	    		printf("BACK RIGHT WHEEL STILL TURNING");
    	    	}
    	    	else if (!FR_swerve_proceed)
    	    	{
    	    		printf("FRONT LEFT WHEEL STILL TURNING");
    	    	}
    	    	else if (!FR_swerve_proceed)
    	    	{
    	    		printf("BACK LEFT WHEEL STILL TURNING");
    	    	}
                else
                {
                	drive_proceed = 0;
                }
    	        break;
    	    case 'Y':
    	    case 'y':
    	    	if (!(FR_swerve_proceed || BR_swerve_proceed || FL_swerve_proceed || BL_swerve_proceed))
    	    	{
    	    		swerve_controller_proceed = FR.swerve_controller('Y', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			FR_swerve_proceed = true;
    	    		}
    	    		swerve_controller_proceed = BR.swerve_controller('Y', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			BR_swerve_proceed = true;
    	    		}
    	    		swerve_controller_proceed = FL.swerve_controller('Y', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			FL_swerve_proceed = true;
    	    		}
    	    		swerve_controller_proceed = BL.swerve_controller('Y', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			BL_swerve_proceed = true;
    	    		}
    	    	}
    	    	else if (!FR_swerve_proceed)
    	    	{
    	    		printf("FRONT RIGHT WHEEL STILL TURNING");
    	    	}
    	    	else if (!BR_swerve_proceed)
    	    	{
    	    		printf("BACK RIGHT WHEEL STILL TURNING");
    	    	}
    	    	else if (!FR_swerve_proceed)
    	    	{
    	    		printf("FRONT LEFT WHEEL STILL TURNING");
    	    	}
    	    	else if (!FR_swerve_proceed)
    	    	{
    	    		printf("BACK LEFT WHEEL STILL TURNING");
    	    	}
                else
                {
                	drive_proceed = 0;
                }
    	        break;
    	    case 'Z':
    	    case 'z':
    	    	if (!(FR_swerve_proceed || BR_swerve_proceed || FL_swerve_proceed || BL_swerve_proceed))
    	    	{
    	    		swerve_controller_proceed = FR.swerve_controller('Z', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			FR_swerve_proceed = true;
    	    		}
    	    		swerve_controller_proceed = BR.swerve_controller('Z', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			BR_swerve_proceed = true;
    	    		}
    	    		swerve_controller_proceed = FL.swerve_controller('Z', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			FL_swerve_proceed = true;
    	    		}
    	    		swerve_controller_proceed = BL.swerve_controller('Z', speed, false);
    	    		if (swerve_controller_proceed == 0) // Success
    	    		{
    	    			BL_swerve_proceed = true;
    	    		}
    	    	}
    	    	else if (!FR_swerve_proceed)
    	    	{
    	    		printf("FRONT RIGHT WHEEL STILL TURNING");
    	    	}
    	    	else if (!BR_swerve_proceed)
    	    	{
    	    		printf("BACK RIGHT WHEEL STILL TURNING");
    	    	}
    	    	else if (!FR_swerve_proceed)
    	    	{
    	    		printf("FRONT LEFT WHEEL STILL TURNING");
    	    	}
    	    	else if (!FR_swerve_proceed)
    	    	{
    	    		printf("BACK LEFT WHEEL STILL TURNING");
    	    	}
                else
                {
                	drive_proceed = 0;
                }
    	        break;
    	    default:
    	    	return MRAA_ERROR_INVALID_PARAMETER;
    	}

    }



};

#endif
