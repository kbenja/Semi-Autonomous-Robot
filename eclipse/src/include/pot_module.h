#include "mraa.h"
#ifndef POT_MODULE_H
#define POT_MODULE_H

class Pot_Module {
    mraa_aio_context adc_port;  //ADC potentiometer is connected to
    uint16_t adc_value;         //adc value

    mraa_gpio_context mux_select;   //analog multiplexer select
    bool is_muxed;
public:
    //default constructor
    Pot_Module() {
        adc_port = mraa_aio_init(0);
        mraa_aio_set_bit(adc_port, 12);
        adc_value = mraa_aio_read(adc_port);        //read port
        printf("Potentiometer module initialized to ADC 0\n");
    }

    //constructor w/ specific ADC port and ADC precision
    Pot_Module(uint8_t port, uint8_t adc_precision, bool adc_mux, int mux_port) {
        adc_port = mraa_aio_init(port);
        mraa_aio_set_bit(adc_port, adc_precision);
        adc_value = mraa_aio_read(adc_port);        //read port

        if(adc_mux) {           //create GPIO context for mux if pot is multiplexed
            is_muxed = true;
            mux_select = mraa_gpio_init(mux_port);      //initialize
            mraa_gpio_dir(mux_select, MRAA_GPIO_OUT);   //set GPIO as output
        }
        else {
            is_muxed = false;
            mux_select = NULL;
        }

        printf("Potentiometer module initialized to ADC %d\n", port);
    }

    //destructor; free aio_context memory
    ~Pot_Module() {
        mraa_aio_close(adc_port);
        mraa_gpio_close(mux_select);
    }


    /*FUNCTIONS*/
    //Averaging 5 values
    uint16_t get_average_val() {
        if(this->is_muxed) {
            mraa_gpio_write(this->mux_select, 0);
        }
        adc_value = mraa_aio_read(adc_port);        //read port
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        return adc_value/5;
    }

    uint16_t get_val() {
        adc_value = mraa_aio_read(adc_port);        //read port
        return adc_value;
    }

};

#endif
