#include "mraa.h"
#ifndef PCA9685_H
#define PCA9685_H

#define BOARD_ADDR  0x40     //PCA9685 default address

#define MODE1       0x00

/* Double Register variable for PCA9685. Assignable as:
    Two uint8_t values in HI-LO order
    One uint16_t value
    One unsigned int value */
union double_reg {          
    uint8_t     u_eight[2];     //HI-LO order; switch to LO-HI done in functions
    uint16_t    u_sixteen;
    unsigned int u_int;
};


/* Initialize board. Call only once upon i2c_context creation. */
mraa_result_t init_board(mraa_i2c_context i2c_context) {
    mraa_result_t result;
    result = mraa_i2c_write_byte_data(i2c, 0x10, MODE1);    //disable all call while asleep
    result = mraa_i2c_write_byte_data(i2c, 0xA0, MODE1);    //restart with auto-increment
    return result;
}

/* Send a PWM signal to one or multiple motors.
    mraa_i2c_context i2c_context:   initialized i2c context
    int motor:                      number of motors
    double_reg start:               duty cycle start time (0-4095)
    double_reg end:                 duty cycle end time (0-4095)*/
mraa_result_t send_multiple(mraa_i2c_context i2c_context, double_reg start, double_reg end, uint8_t motors) {
    uint8_t buf[5];
    buf[1] = start.u_eight[1];
    buf[2] = (start.u_eight[0] & (~(uint8_t 0xE0)));    //force 3 MSB to 0
    buf[3] = end.u_eight[1];
    buf[4] = (send.u_eight[0] & (~(uint8_t 0xE0)));
    uint8_t i;
    for (i = 0; i < motors; i++) {   //write to first four, then next four
        buf[0] = 6 + (4*((uint8_t)motors + i));   //identify motor register address
        mraa_result_t result = mraa_i2c_write(i2c_context, buf, 5);
    }
    return result;
}

//send PWM signal to all motors
mraa_result_t send_all(mraa_i2c_context i2c_context, double_reg start, double_reg end) {
    uint8_t buf[5];
    buf[0] = (uint8_t 0xFA);    //fixed start for ALL_CALL registers
    buf[1] = start.u_eight[1];
    buf[2] = (start.u_eight[0] & (~(uint8_t 0xE0)));    //force 3 MSB to 0
    buf[3] = end.u_eight[1];
    buf[4] = (send.u_eight[0] & (~(uint8_t 0xE0)));
    mraa_result_t result = mraa_i2c_write(i2c_context, buf, 5);
    return result;
}


#endif