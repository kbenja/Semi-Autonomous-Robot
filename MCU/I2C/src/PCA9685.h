/* 
    PCA9685 header file

*/

/*
 * PCA9685.h
 *
 *  Created on: Feb 14, 2016
 *      Author: adwong16
 */

#include "mcu_api.h"
#include "mcu_errno.h"

#ifndef PCA9685_H_
#define PCA9685_H_

#define PCA9685_ADDR 0x40	//I2C address

//


unsigned char led_on = '\x10';

/*
       LO       HI      Register layout order
       HI       LO      Logical Order
    XXXXXXXX_XXXXXXXX
       
*/


/*
    Input:
        reg:    The LO register number (numbering is LO, HI)
        value:  A two-character string containing hex values to be written (in HI, LO order)
    Output:
        values written to register on PCA9685
*/
/*
void pwm_write(int reg, char* value) {
    int send;
    send = i2c_write(PCA9685_ADDR, reg, value[1], 1);       //LO
    send = i2c_write(PCA9685_ADDR, (reg + 1), value[0], 1); //HI
}
*/
/*
    Input:
        reg:    The LO ON register number (numbering is LO, HI, and they are in ON, OFF order)
        value:  A two-character string containing hex values to be written (in ON_HI, ON_LO, OFF_HI, OFF_LO order)
*/

/*
void pwm_write_all(int reg, char* value) {
    int send;
    send = i2c_write(PCA9685_ADDR, reg, value[1], 1);       //ON_LO
    send = i2c_write(PCA9685_ADDR, (reg + 1), value[0], 1); //ON_HI
    send = i2c_write(PCA9685_ADDR, (reg + 2), value[3], 1); //OFF_LO
    send = i2c_write(PCA9685_ADDR, (reg + 3), value[2], 1); //OFF_HI
}
*/

/*
    Input: 
        none
    Output:
        zero values written to all PWM registers (i.e. all stop)
*/
/*
void stop_all() {
    ;
}
*/

//register numbers
#define LED0_ON     0x06
#define LED0_OFF    0x08
#define LED0_ON_L   0x06
#define LED0_ON_H   0x07
#define LED0_OFF_L  0x08
#define LED0_OFF_H  0x09

#define LED1_ON     0x0A
#define LED1_OFF    0x0C
#define LED1_ON_L   0x0A
#define LED1_ON_H   0x0B
#define LED1_OFF_L  0x0C
#define LED1_OFF_H  0x0D

#define LED2_ON     0x0E
#define LED2_OFF    0x10
#define LED2_ON_L   0x0E
#define LED2_ON_H   0x0F
#define LED2_OFF_L  0x10
#define LED2_OFF_H  0x11

#define LED3_ON     0x12
#define LED3_OFF    0x14
#define LED3_ON_L   0x12
#define LED3_ON_H   0x13
#define LED3_OFF_L  0x14
#define LED3_OFF_H  0x15

#define LED4_ON_L   0x16
#define LED4_ON_H   0x17
#define LED4_OFF_L  0x18
#define LED4_OFF_H  0x19

#define LED0_ON     0x06
#define LED0_OFF    0x08
#define LED5_ON_L   0x1A
#define LED5_ON_H   0x1B
#define LED5_OFF_L  0x1C
#define LED5_OFF_H  0x1D

#define LED6_ON_L   0x1E
#define LED6_ON_H   0x1F
#define LED6_OFF_L  0x20
#define LED6_OFF_H  0x21

#define LED7_ON_L   0x22
#define LED7_ON_H   0x23
#define LED7_OFF_L  0x24
#define LED7_OFF_H  0x25

#define LED8_ON_L   0x26
#define LED8_ON_H   0x27
#define LED8_OFF_L  0x28
#define LED8_OFF_H  0x29

#define LED9_ON_L   0x2A
#define LED9_ON_H   0x2B
#define LED9_OFF_L  0x2C
#define LED9_OFF_H  0x2D

#define LED10_ON_L  0x2E
#define LED10_ON_H  0x2F
#define LED10_OFF_L 0x30
#define LED10_OFF_H 0x31

#define LED11_ON_L  0x32
#define LED11_ON_H  0x33
#define LED11_OFF_L 0x34
#define LED11_OFF_H 0x35

#define LED12_ON_L  0x36
#define LED12_ON_H  0x37
#define LED12_OFF_L 0x38
#define LED12_OFF_H 0x39

#define LED13_ON_L  0x3A
#define LED13_ON_H  0x3B
#define LED13_OFF_L 0x3C
#define LED13_OFF_H 0x3D

#define LED14_ON_L  0x3E
#define LED14_ON_H  0x3F
#define LED14_OFF_L 0x40
#define LED14_OFF_H 0x41

#define LED15_ON_L  0x42
#define LED15_ON_H  0x43
#define LED15_OFF_L 0x44
#define LED15_OFF_H 0x45
#endif /* PCA9685_H_ */
