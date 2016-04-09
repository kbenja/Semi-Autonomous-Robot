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
        mraa_aio_set_bit(adc_port, 12);
        adc_value = mraa_aio_read(adc_port);        //read port
        printf("Potentiometer module initialized to ADC 0\n");
    }

    //constructor w/ specific ADC port and ADC precision
    Pot_Module(uint8_t port, uint8_t adc_precision) {
        adc_port = mraa_aio_init(port);
        mraa_aio_set_bit(adc_port, adc_precision);
        adc_value = mraa_aio_read(adc_port);        //read port
        printf("Potentiometer module initialized to ADC %d\n", port);
    }

    //destructor; free aio_context memory
    ~Pot_Module() {
        mraa_aio_close(adc_port);
    }


    /*FUNCTIONS*/
    //Averaging 10 values
    uint16_t get_average_val() {
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
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);

        return adc_value/10;

        /*
        New averaging function; averages 8 values
            and divides by shifting right 3 bits.
            Potentially faster.
        */
        /*
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);
        usleep(50);
        adc_value += mraa_aio_read(adc_port);

        return adc_value >> 3;      //shift right 3 bits = divide by 2^3 = divide by 8
        */

    }

    uint16_t get_val() {
        adc_value = mraa_aio_read(adc_port);        //read port
        return adc_value;
    }

};

#endif
