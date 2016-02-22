#include "mcu_api.h"
#include "mcu_errno.h"
#include <string.h>
/*
* This program will blink GPIO 48, which connects to Edison Arduino board DIG 7
*/

//convert Arduino ports to Intel MCU ports


void mcu_main()
{
    /* your configuration code starts here */
    char buf[64];
    int len;

    gpio_setup(48, 1);  /* set GPIO 48 as output */
	gpio_setup(182, 1);
	gpio_setup(13, 1);
	gpio_setup(129, 1);

    while (1)       /* your loop code starts here */
    {
        do {
            len = host_receive((unsigned char *)buf, 64);
            mcu_sleep(10);
        } while (len <= 0);
        if (strncmp(buf, "U", 1) == 0)
        {
        	gpio_write(48, 1);
			gpio_write(182, 0);
			gpio_write(13, 0);
			gpio_write(129, 0);
        }
        else if (strncmp(buf, "D", 1) == 0)
		{
			gpio_write(48, 0);
			gpio_write(182, 1);
			gpio_write(13, 0);
			gpio_write(129, 0);
		}
        else if (strncmp(buf, "L", 1) == 0)
		{
			gpio_write(48, 0);
			gpio_write(182, 0);
			gpio_write(13, 1);
			gpio_write(129, 0);
		}
        else if (strncmp(buf, "R", 1) == 0)
		{
			gpio_write(48, 0);
			gpio_write(182, 0);
			gpio_write(13, 0);
			gpio_write(129, 1);
		}
    }
}
