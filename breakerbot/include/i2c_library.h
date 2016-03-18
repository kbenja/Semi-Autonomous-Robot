#include "mraa.h"
#include "stdint.h"
#ifndef I2C_LIBRARY
#define I2C_LIBRARY

//PCA9685 default address
#define BOARD_ADDR  0x40

//control registers
#define MODE1       0x00
#define MODE2       0x01

//PWM duration registers
#define OUT0_ON_L   0x06
#define OUT0_ON_H   0x07
#define OUT0_OFF_L  0x08
#define OUT0_OFF_H  0x09
#define OUT1_ON_L   0x0A
#define OUT1_ON_H   0x0B
#define OUT1_OFF_L  0x0C
#define OUT1_OFF_H  0x0D
#define OUT2_ON_L   0x0E
#define OUT2_ON_H   0x0F
#define OUT2_OFF_L  0x10
#define OUT2_OFF_H  0x11
#define OUT3_ON_L   0x12
#define OUT3_ON_H   0x13
#define OUT3_OFF_L  0x14
#define OUT3_OFF_H  0x15
#define OUT4_ON_L   0x16
#define OUT4_ON_H   0x17
#define OUT4_OFF_L  0x18
#define OUT4_OFF_H  0x19
#define OUT5_ON_L   0x1A
#define OUT5_ON_H   0x1B
#define OUT5_OFF_L  0x1C
#define OUT5_OFF_H  0x1D
#define OUT6_ON_L   0x1E
#define OUT6_ON_H   0x1F
#define OUT6_OFF_L  0x20
#define OUT6_OFF_H  0x21
#define OUT7_ON_L   0x22
#define OUT7_ON_H   0x23
#define OUT7_OFF_L  0x24
#define OUT7_OFF_H  0x25
#define OUT8_ON_L   0x26
#define OUT8_ON_H   0x27
#define OUT8_OFF_L  0x28
#define OUT8_OFF_H  0x29
#define OUT9_ON_L   0x2A
#define OUT9_ON_H   0x2B
#define OUT9_OFF_L  0x2C
#define OUT9_OFF_H  0x2D
#define ALL_ON_L    0xFA
#define ALL_ON_H    0xFB
#define ALL_OFF_L   0xFC
#define ALL_OFF_H   0xFD

//MODE1 control bits
#define RESTART     0x80
#define EXTCLK      0x40
#define AUTO_INC    0x20
#define SLEEP       0x10
#define SUB1        0x08
#define SUB2        0x04
#define SUB3        0x02
#define ALLCALL     0x01

//PWM duration quick-access values
#define FULL_ON     0x10
#define FULL_OFF    0x10

//PWM master control register (all PWM outputs
#define ALL_ON      0x10
#define ALL_OFF     0x10

#define ENABLE      0x10

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
mraa_result_t i2c_init_board(mraa_i2c_context i2c_context) {
    mraa_result_t result;
    result = mraa_i2c_write_byte_data(i2c_context, SLEEP, MODE1);                   //disable all call while asleep
    result = mraa_i2c_write_byte_data(i2c_context, ALL_OFF, ALL_OFF_H);             //turn off all PWM outputs
    result = mraa_i2c_write_byte_data(i2c_context, (RESTART | AUTO_INC), MODE1);    //restart with auto-increment
    return result;
}

/* Send a PWM signal to one or multiple motors.
    mraa_i2c_context i2c_context:   initialized i2c context
    int motor:                      number of motors (0 --> motors)
    double_reg start:               duty cycle start time (0-4095)
    double_reg end:                 duty cycle end time (0-4095)*/
mraa_result_t i2c_send_multiple(mraa_i2c_context i2c_context, double_reg start, double_reg end, uint8_t motors) {
    uint8_t buf[5];
    buf[1] = start.u_eight[1];
    buf[2] = (start.u_eight[0] & (~((uint8_t) 0xE0)));    //force 3 MSB to 0
    buf[3] = end.u_eight[1];
    buf[4] = (end.u_eight[0] & (~((uint8_t) 0xE0))); //start or end?
    uint8_t i;
    mraa_result_t result;
    for (i = 0; i < motors; i++) {   //write to first four, then next four
        buf[0] = 6 + (4*((uint8_t)motors + i));   //identify motor register address
        result = mraa_i2c_write(i2c_context, buf, 5);
    }
    return result;
}

/* Send a PWM signal to all motors
    mraa_i2c_context i2c_context:   initialized i2c context
    double_reg start:               duty cycle start time (0-4095)
    double_reg end:                 duty cycle end time (0-4095)*/
mraa_result_t i2c_send_all(mraa_i2c_context i2c_context, double_reg start, double_reg end) {
    uint8_t buf[5];
    buf[0] = ((uint8_t) 0xFA);    //fixed start for ALL_PWM registers
    buf[1] = start.u_eight[1];
    buf[2] = (start.u_eight[0] & (~((uint8_t) 0xE0)));    //force 3 MSB to 0
    buf[3] = end.u_eight[1];
    buf[4] = (end.u_eight[0] & (~((uint8_t) 0xE0)));
    mraa_result_t result = mraa_i2c_write(i2c_context, buf, 5);
    return result;
}

/* Send a PWM signal to one motor
    mraa_i2c_context i2c_context:   initialized i2c context
    uint8_t reg:                    which motor to send signal to
    double_reg end:                 duty cycle end time (0-4095)*/
mraa_result_t i2c_send_signal(mraa_i2c_context i2c_context, uint8_t reg, double_reg end) {

    mraa_result_t status = MRAA_SUCCESS;

    printf("Bits reversed = 0x%04x\n", end.u_sixteen);
    printf("Register = 0x%02x\n", reg);
    printf("Sending = 0x%02x to register 0x%02x\n", end.u_eight[0], reg + 0x01);
    printf("Sending = 0x%02x to register 0x%02x\n", end.u_eight[1], reg);

    // set high bit
    status = mraa_i2c_write_byte_data(i2c_context, end.u_eight[0], reg + 0x01);
    // set low bit
    status = mraa_i2c_write_byte_data(i2c_context, end.u_eight[1], reg);
    return status;

}

/* Disable PWM output on all motors by forcing all outputs to off
    mraa_i2c_context i2c_context:   initialized i2c context */
mraa_result_t i2c_all_off(mraa_i2c_context i2c_context) {
    uint8_t buf[2];
    buf[0] = (uint8_t)ALL_OFF_H;    //fixed start for ALL_PWM_OFF_H register
    buf[1] = (uint8_t)ENABLE;       //0001_0000 = All outputs off
    mraa_result_t result = mraa_i2c_write(i2c_context, buf, 2);
    return result;
}

/* Disable PWM output on all motors by pausing oscillator. Unlike all_off, does not affect PWM values
    mraa_i2c_context i2c_context:   initialized i2c context */
mraa_result_t i2c_pause(mraa_i2c_context i2c_context) {
    uint8_t buf[2];
    buf[0] = (uint8_t)MODE1;                //MODE1 register
    buf[1] = (uint8_t)(AUTO_INC | SLEEP);   //0001_0000 = All outputs off
    mraa_result_t result = mraa_i2c_write(i2c_context, buf, 2);
    return result;
}

/* Enable PWM output on all motors by resuming oscillator.
    mraa_i2c_context i2c_context:   initialized i2c context */
mraa_result_t i2c_resume(mraa_i2c_context i2c_context) {
    uint8_t buf[2];
    buf[0] = (uint8_t)MODE1;        //MODE1 register
    buf[1] = (uint8_t)AUTO_INC;     //0001_0000 = All outputs off
    mraa_result_t result = mraa_i2c_write(i2c_context, buf, 2);
    return result;
}


#endif
