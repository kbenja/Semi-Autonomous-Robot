#include "mraa/aio.h"
#include "i2c_library.h"
#include <inttypes.h>

#ifndef MOTOR_MODULE_H
#define MOTOR_MODULE_H

class Motor_Module {
// move this into main function
uint8_t start_reg;

public:
    Motor_Module() {
        start_reg = 0x06;
    }

    Motor_Module(uint8_t reg) {
        start_reg = reg;
    }

    mraa_result_t send_signal(mraa_i2c_context i2c_context, double_reg signal)   {
        printf("Sending %" PRIu16 " to register %" PRIu8 "\n", signal.u_sixteen, start_reg);
        mraa_result_t result = MRAA_SUCCESS;
        result = i2c_send_signal(i2c_context, start_reg, signal);
        return result;
    }

    // mraa_result_t turn_off(mraa_i2c_context i2c_context, double_reg end) {
    //     // turns off the motor
    // }
};

#endif
