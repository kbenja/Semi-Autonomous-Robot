#include <fstream>
#include <unistd.h>
#include "mraa/aio.h"

#ifndef LIDAR_MODULE_H
#define LIDAR_MODULE_H

class Lidar_Module {
    int analog_input;
    mraa_aio_context adc_input;

    mraa_gpio_context mux_select;   //analog multiplexer select
    bool is_muxed;
public:
    Lidar_Module()
    {
        //analog input is A0 by default
        analog_input = 0;
        adc_input = mraa_aio_init(analog_input);
        printf("initialized lidar module to on AI%d", analog_input);

        is_muxed = false;
        mux_select = NULL;
    };
    Lidar_Module(int port, bool adc_mux, int mux_port)
    {
        analog_input = port;
        adc_input = mraa_aio_init(analog_input);
        mraa_aio_set_bit(adc_input, 12);//add

        if(adc_mux) {           //create GPIO context for mux if pot is multiplexed
            is_muxed = true;
            mux_select = mraa_gpio_init(mux_port);      //initialize
            mraa_gpio_dir(mux_select, MRAA_GPIO_OUT);   //set GPIO as output
        }
        else {
            is_muxed = false;
            mux_select = NULL;
        }

        printf("initialized lidar module to on AI%d\n", analog_input);
    }

    ~Lidar_Module() {
        mraa_aio_close(adc_input);
        mraa_gpio_close(mux_select);
    }

    //CURRENT DISTANCE
    float get_distance_reading()
    {
        if(this->is_muxed) {
            mraa_gpio_write(this->mux_select, 1);
        }
        float adc_value_float = 0.0;
        // calibrate and adjust this constant
        float calibrate_constant = 1.8181;
        adc_value_float = mraa_aio_read_float(adc_input)*calibrate_constant;

        return adc_value_float;
    }
    //AVERAGE CURRENT DISTANCE IN uint16_t*** RAW ADC VALUE
    uint16_t get_average_distance_reading_int()
    {
        if(this->is_muxed) {
            mraa_gpio_write(this->mux_select, 1);
        }
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
        if(this->is_muxed) {
            mraa_gpio_write(this->mux_select, 1);
        }
        uint16_t adc_value_int = 0;
        // calibrate and adjust this constant
        //uint16_t calibrate_constant_int = 55.0;
        adc_value_int = mraa_aio_read(adc_input);

        return adc_value_int;
    }
    //AVERAGE CURRENT DISTANCE
    float get_average_distance_reading()
    {
        if(this->is_muxed) {
            mraa_gpio_write(this->mux_select, 1);
        }
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

