#include "mraa.h"
#ifndef PCA9685_H
#define PCA9685_H

#define BOARD_ADDR 0x40     //PCA9685 default address

union double_reg {          //two sequential uint8_t registers, selectable as 
    uint8_t     u_eight[2]; //HI-LO order; switch to LO-HI done in functions
    uint16_t    u_sixteen;
    unsigned int u_int;
};

//send PWM signal to multiple motors
mraa_result_t send_multiple(mraa_i2c_context i2c_context, int motor; double_reg start, double_reg end, uint8_t motors) {
    uint8_t buf[9];
    buf[1] = start.u_eight[1];
    buf[2] = (start.u_eight[0] & (~(uint8_t 0xE0)));    //force 3 MSB to 0
    buf[3] = end.u_eight[1];
    buf[4] = (send.u_eight[0] & (~(uint8_t 0xE0)));
    uint8_t i;
    for (i = 0; i < motors; i++) {   //write to first four, then next four
        buf[0] = 6 * ((uint8_t)motor + i);   //identify motor register address
        mraa_result_t result = mraa_i2c_write(i2c_context, buf, 5);
    }
    return result;
}

//send PWM signal to all motors

mraa_result_t send_all(mraa_i2c_context i2c_context, double_reg start, double_reg end) {
    

}


#endif