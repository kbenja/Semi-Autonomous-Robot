#include "mraa.h"
#include "i2c_library.h"

#ifndef LIDARLITE_MODULE_H
#define LIDARLITE_MODULE_H


#define CTRL_REG 0x00       //control register
#define DIST_REG_H 0x0f     //distance (high-order)
#define DIST_REG_L 0x10     //distance (low-order)
#define LIDAR_TRIG 0x04     //trigger command

#define LIDAR_ADDR 0x62     //LidarLITE I2C address


class LidarLITE_Module {
public:
    uint16_t read_value;
    mraa_i2c_context i2c;   //i2c context

    /*
        Default constructor.
    */
    LidarLITE_Module() {
        //initialize i2c
        i2c = mraa_i2c_init(6);                 //init i2c context
        read_value = 0;

    }
    /*
        @param  mraa_i2c_context i2c_bus:   i2c context (bus) used to communicate

        @returns: A LidarLITE_Module object
    */
    LidarLITE_Module(mraa_i2c_context i2c_bus) {
        //initialize i2c
        i2c = i2c_bus;                 //init i2c context
        read_value = 0;
    }


    /*
        Calibration:
            >50cm:  No calibration needed; averaging function?

    */

    /*
        Takes a LIDAR_Lite measurement.
        @returns:   Measured distance in centimeters as a uint16_t value
    */
    uint16_t get_distance_reading_int() {
        mraa_i2c_address(i2c, LIDAR_ADDR);                              //set address to 0x62
        mraa_i2c_write_byte_data(i2c, LIDAR_TRIG, CTRL_REG);            //write trigger command to control register
        usleep(20000);                                                  //wait 20ms for measurement
        read_value = mraa_i2c_read_byte_data(i2c, DIST_REG_H);    //get high order byte
        read_value <<= 8;                                                  //shift left 8 bits = multiply by 256
        read_value += mraa_i2c_read_byte_data(i2c, DIST_REG_L);            //get low order byte
        return read_value;
    }
};


#endif
