#include "mraa/aio.h"
#include "i2c_library.h"

#ifndef MOTOR_MODULE_H
#define MOTOR_MODULE_H

class Motor_Control {
// move this into main function
mraa_i2c_context i2c;

public:
    Motor_Control() {
        mraa_i2c_context i2c = mraa_i2c_init(6);
    }


    mraa_result_t initialize_motor(mraa_i2c_context i2c_context, uint8_t end) {
        // sends signal
    }

    mraa_result_t turn_all_off(mraa_i2c_context i2c_context, double_reg end) {
        // turns off the motor
    }
};

#endif
