#include "mraa/aio.h"
#include "i2c_library.h"

#ifndef MOTOR_MODULE_H
#define MOTOR_MODULE_H

class Motor_Module {
uint8_t end_reg;
public:

    /**
        Initializes motor module port 1
        @param int port     decides which port to talk to
    */
    Motor_Module() {
        end_reg = 0x0C;
        printf("Motor module initialized to port 1 (register 0x%02x)\n", end_reg);
    }

    /**
        Initializes motor module to specific port
        @param int port     decides which port to talk to
    */
    Motor_Module(int port) {
        end_reg = 0x0C + (port - 1)*4;
        printf("Motor module initialized to port 1 (register 0x%02x)\n", end_reg);
    }

    /**
        Sends signal to single motor

        @param mraa_i2c_context i2c_context:    initialized i2c context
        @param float rate:                      decides the speed and direction of the wheel
        @return mraa_result_t                   should equal MRAA_SUCCESS if no errors
    */
    mraa_result_t send_signal(mraa_i2c_context i2c_context, float rate) {

        mraa_result_t result = MRAA_SUCCESS;
        result = i2c_send_signal(i2c_context, end_reg, rate);
        if(result != MRAA_SUCCESS)
        {
            printf("Could not write to register 0x%02x\n", end_reg);
            return result;
        }

        return result;
    }

};

#endif
