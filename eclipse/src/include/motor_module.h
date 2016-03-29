#include "mraa/aio.h"
#include "i2c_library.h"

#ifndef MOTOR_MODULE_H
#define MOTOR_MODULE_H

/**
    Converts float to value to send to i2c->PWM chip
    @param float start     -1.0 = full CCW, 1.0 = full CW, 0.0 = stop
    @return final_union
*/
double_reg float_to_PWM(float start) {
    if(start > 1.0) {
        start = 1.0;
    } else if (start < -1.0) {
        start = -1.0;
    }
    double_reg final;
    final.u_sixteen = ((uint16_t) 2816-(start*768.0));
    // printf("\nSTART %f becomes 0x%04x \n", start, final.u_sixteen);
    return final;
}


class Motor_Module {
uint8_t end_reg;
public:

    /**
        Initializes motor module port 1
        @param int port     decides which port to talk to
    */
    Motor_Module() {
        end_reg = 0x0C;
        printf("[ init ] Motor module initialized to port 1 (register 0x%02x)\n", end_reg);
    }

    /**
        Initializes motor module to specific port
        @param int port     decides which port to talk to
    */
    Motor_Module(int port) {
        end_reg = 0x0C + (port - 1)*4;
        printf("[ init ] Motor module initialized to port %d (register 0x%02x)\n", port, end_reg);
    }

    /**
        Sends signal to single motor

        @param mraa_i2c_context i2c_context:    initialized i2c context
        @param float rate:                      decides the speed and direction of the wheel
        @return mraa_result_t                   should equal MRAA_SUCCESS if no errors
    */
    mraa_result_t send_signal(const mraa_i2c_context & i2c_context, float speed) {
        double_reg signal = float_to_PWM(speed);

        mraa_result_t result = MRAA_SUCCESS;
        result = i2c_send_signal(i2c_context, end_reg, signal);
        if(result != MRAA_SUCCESS)
        {
            printf("[ !!!! ] Could NOT write to register 0x%02x\n", end_reg);
        } else {
            printf("[ send ] Write signal 0x%04x to register 0x%02x\n", signal.u_sixteen, end_reg);
        }

        return result;
    }

};

#endif
