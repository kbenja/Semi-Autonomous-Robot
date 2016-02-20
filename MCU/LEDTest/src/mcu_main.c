#include "mcu_api.h"
#include "mcu_errno.h"
/*
* This program will blink GPIO 48, which connects to Edison Arduino board DIG 7
*/


void mcu_main()
{
	int confirm = 0;
	unsigned char on = 0x10;	//on value
	unsigned char off = 0x00;	//off value

    while(1)
    {
    	confirm = i2c_write(0x40, ((unsigned char)0xFB), &on, 1);
    	mcu_sleep(100);
    	confirm = i2c_write(0x40, ((unsigned char)0xFB), &off, 1);
    	mcu_sleep(100);
    }
}


/*

 void mcu_main()
{
    // your configuration code starts here
    gpio_setup(48, 1);  // set GPIO 48 as output
    while (1)
    {
        gpio_write(48, 1);
        mcu_sleep(100);         // sleep 1 second
        gpio_write(48, 0);
        mcu_sleep(100);
    }
}
*/
