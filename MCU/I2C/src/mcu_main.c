#include "mcu_api.h"
#include "mcu_errno.h"
#include "PCA9685.h"



//00100000


void mcu_main()
{
    /* your configuration code starts here */


    //test write to individual registers by turning all LEDs on
    int confirm;
    confirm = i2c_write(PCA9685_ADDR, LED0_ON_H, '\x10', 1);    //Write value 00010000 to LED0_ON_H
    confirm = i2c_write(PCA9685_ADDR, LED1_ON_H, '\x10', 1);    //Write value 00010000 to LED1_ON_H
    confirm = i2c_write(PCA9685_ADDR, LED2_ON_H, '\x10', 1);    //Write value 00010000 to LED2_ON_H
    confirm = i2c_write(PCA9685_ADDR, LED3_ON_H, '\x10', 1);    //Write value 00010000 to LED3_ON_H

    //test write to sequential registers (LO-HI, ON-OFF)
    confirm = i2c_write(PCA9685_ADDR, LED0_ON, '\x00\x00\x20\x00', 4);



	int confirm = i2c_write(PCA9685_ADDR, LED0_ON_L, ,1);


    while (1)       /* your loop code starts here */
    {
    }
}
