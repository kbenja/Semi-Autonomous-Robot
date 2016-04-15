#include "mraa.h"

#ifndef NAVX_MODULE_H
#define NAVX_MODULE_H

class NavX_Module {
int offset;
int actual;
public:
    NavX_Module() {
        offset = 0.0;
    }
    int get_yaw() {
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t result = MRAA_SUCCESS;
        result = mraa_i2c_address(i2c, 0x32);

        if(result != MRAA_SUCCESS){
            // printf("was not able to connect to address\n");
            return -2;
        }

        uint8_t high_bits;
        uint8_t low_bits;
        signed_double_reg combined;
        int status = 1;

        status = mraa_i2c_write_byte(i2c, 0x16);
        status = mraa_i2c_read(i2c, &high_bits, 1);

        status = mraa_i2c_write_byte(i2c, 0x17);
        status = mraa_i2c_read(i2c, &low_bits, 1);

        if(status != 1) {
            // printf("Could not read from the navx board\n");
            return -1;
        }

        combined.upper = high_bits;
        combined.lower = low_bits;
        int final = combined.sixteen;
        if (final < 0) {
            final = 36000 + combined.sixteen;
        }
        actual = final; // set reference to actual orientation (used in set_zero)
        // return (final + offset)%36000;
        return final/100;
    }

    void set_zero() {
        offset = (36000 - actual);
    }
};

#endif
