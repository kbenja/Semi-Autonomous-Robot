#include "pot_module.h"
#include "mraa.h"

#ifndef POT_MODULE_H
#define POT_MODULE_H

class Pot_Module {
    mraa_aio_context adc_port;  //ADC potentiometer is connected to
    uint16_t adc_value;
public:
    //default constructor
    Pot_Module() {
        adc_port = mraa_aio_init(0);
        mraa_aio_set_bit(adc_port, 10);
        printf("Potentiometer module initialized to ADC 0\n", adc_port);
    }

    //constructor w/ specific ADC port and ADC precision
    Pot_Module(uint8_t port, uint8_t adc_precision) {
        adc_port = mraa_aio_init(port);
        mraa_aio_set_bit(adc_port, adc_precision);
        printf("Potentiometer module initialized to ADC %d\n", port);
    }

    //destructor; free aio_context memory
    ~Pot_Module() {
        mraa_aio_close(adc_port);
    }


    /*FUNCTIONS*/
    uint16_t get_val() {
        adc_value = mraa_aio_read(adc_port);        //read port
        return adc_value;
    }

};

#endif