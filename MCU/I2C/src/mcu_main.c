#include "mcu_api.h"
#include "mcu_errno.h"
#include "PCA9685.h"



//00100000
void i2c_error_check(int ret_val) {
    if (ret_val) {
        debug_print(DBG_ERROR, "I2C failed to transmit\n");
    }
}
/*
void mcu_main()
{
    /* your configuration code starts here


    //test write to individual registers by turning all LEDs on









    int confirm;




    //confirm = i2c_write(PCA9685_ADDR, LED1_ON_H, '\x10', 1);    //Write value 00010000 to LED1_ON_H
    //confirm = i2c_write(PCA9685_ADDR, LED2_ON_H, '\x10', 1);    //Write value 00010000 to LED2_ON_H
    //confirm = i2c_write(PCA9685_ADDR, LED3_ON_H, '\x10', 1);    //Write value 00010000 to LED3_ON_H

    //test write to sequential registers (LO-HI, ON-OFF)
    //confirm = i2c_write(PCA9685_ADDR, LED0_ON, '\x00\x00\x20\x00', 4);



	//int confirm = i2c_write(PCA9685_ADDR, LED0_ON_L, ,1);


    while (1)
    {
        confirm = i2c_write(PCA9685_ADDR, LED0_ON_H, &led_on, 1);    //Force LED to be always on
        i2c_error_check(confirm);
        confirm = i2c_write(PCA9685_ADDR, LED0_ON_L, '\x00', 1);
        i2c_error_check(confirm);
        confirm = i2c_write(PCA9685_ADDR, LED0_OFF_H, '\x00', 1);   //Off time is 0
        i2c_error_check(confirm);
        confirm = i2c_write(PCA9685_ADDR, LED0_OFF_L, '\x00', 1);
        i2c_error_check(confirm);
    }
}
*/



/* the I2C address of MPU6050 */
#define MPU6050_ADDR    0x40
/* the register address of WHOAMI register, which is used to verify
the identity of the device */
#define MPU6050_WHOAMI          ((unsigned char)0x06)


static unsigned char mpu6050_read_whoami()
{
    unsigned char id;
    int res;
    res = i2c_read(MPU6050_ADDR, MPU6050_WHOAMI, &id, 1);
    if (res)
    {
        debug_print(DBG_ERROR, "mpu6050_i2c_read whoami fail...");
        return 0Xff;
    }

    return id;
}

void mcu_main()
{
    unsigned char device_id;
    char buf[16];
    int len;
    while (1)
    {
        mcu_sleep(300);
        debug_print(DBG_INFO, "after sleep 3 seconds\n");
        device_id = mpu6050_read_whoami();
        len = mcu_snprintf(buf, 16, "id = %d\n", device_id);
        host_send((unsigned char*)buf, len);
    }
}
