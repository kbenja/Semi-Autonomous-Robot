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
        //analog input is A00 by default
        analog_input = 0;
        adc_input = mraa_aio_init(analog_input);
        printf("initialized lidar module to on AI%d", analog_input);
    };
    Lidar_Module(int port)
    {
        analog_input = port;
        adc_input = mraa_aio_init(analog_input);
        printf("initialized lidar module to on AI%d\n", analog_input);
    }
    float get_distance_reading()
    {
        float adc_value_float = 0.0;
        // calibrate and adjust this constant
        float calibrate_constant = 55.0;
        adc_value_float = mraa_aio_read_float(adc_input)*calibrate_constant;

        return adc_value_float;
    }
};

#endif

