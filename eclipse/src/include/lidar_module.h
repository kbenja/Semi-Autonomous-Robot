#include <fstream>
#include <unistd.h>
#include "mraa/aio.h"

#ifndef LIDAR_MODULE_H
#define LIDAR_MODULE_H

class Lidar_Module
{
int analog_input;
mraa_aio_context adc_input;
public:
    Lidar_Module()
    {
        //analog input is A0 by default
        analog_input = 0;
        adc_input = mraa_aio_init(analog_input);
        printf("initialized lidar module to on AI%d", analog_input);
    };
    Lidar_Module(int port)
    {
        analog_input = port;
        adc_input = mraa_aio_init(analog_input);
        mraa_aio_set_bit(adc_input, 12);//add
        printf("initialized lidar module to on AI%d\n", analog_input);
    }

    //CURRENT DISTANCE
    float get_distance_reading()
    {
        float adc_value_float = 0.0;
        // calibrate and adjust this constant
        float calibrate_constant = 1.8181;
        adc_value_float = mraa_aio_read_float(adc_input)*calibrate_constant;

        return adc_value_float;
    }
    //AVERAGE CURRENT DISTANCE IN uint16_t*** RAW ADC VALUE
    uint16_t get_average_distance_reading_int()
    {
        uint16_t adc_value_int = 0;
        // calibrate and adjust this constant
        //uint16_t calibrate_constant_int = 55.0;
        adc_value_int += mraa_aio_read(adc_input);
        usleep(50);
        adc_value_int += mraa_aio_read(adc_input);
        usleep(50);
        adc_value_int += mraa_aio_read(adc_input);
        usleep(50);
        adc_value_int += mraa_aio_read(adc_input);
        usleep(50);
        adc_value_int += mraa_aio_read(adc_input);
        usleep(50);

        return adc_value_int/5;
    }
    //CURRENT DISTANCE IN uint16_t
    uint16_t get_distance_reading_int()
    {
        uint16_t adc_value_int = 0;
        // calibrate and adjust this constant
        //uint16_t calibrate_constant_int = 55.0;
        adc_value_int = mraa_aio_read(adc_input);

        return adc_value_int;
    }
    //AVERAGE CURRENT DISTANCE
    float get_average_distance_reading()
    {
        float adc_value_float = 0.0;
        // calibrate and adjust this constant
        float calibrate_constant = 1.8181;

        adc_value_float += mraa_aio_read_float(adc_input)*calibrate_constant;
        usleep(50);
        adc_value_float += mraa_aio_read_float(adc_input)*calibrate_constant;
        usleep(50);
        adc_value_float += mraa_aio_read_float(adc_input)*calibrate_constant;
        usleep(50);
        adc_value_float += mraa_aio_read_float(adc_input)*calibrate_constant;
        usleep(50);
        adc_value_float += mraa_aio_read_float(adc_input)*calibrate_constant;
        usleep(50);

        return adc_value_float/5;
    }


};

#endif

