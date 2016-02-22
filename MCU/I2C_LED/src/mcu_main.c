#include "mcu_api.h"
#include "mcu_errno.h"


#define PCA9685_ADDR	0x40
#define LED_ON_H		0xFB


//00100000


void toggle_LEDs(int status) {
	int confirm;
	unsigned char on = 0x10;	//on value
	unsigned char off = 0x00;	//off value

	if (status == 1) {
		confirm = i2c_write(PCA9685_ADDR, ((unsigned char)LED_ON_H), &on, 1);
	}
	else if (status == 0) {
		confirm = i2c_write(PCA9685_ADDR, ((unsigned char)LED_ON_H), &off, 1);
	}
}

void mcu_main()
{
     while(1)
    {
    	toggle_LEDs(1);
    	mcu_sleep(100);
    	toggle_LEDs(0);
    	mcu_sleep(100);
    }
}
