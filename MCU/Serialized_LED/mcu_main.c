#include "mcu_api.h"
#include "mcu_errno.h"
#include <string.h>

void mcu_main()
{
    gpio_setup(13, 1); // pin 5 = right
    gpio_setup(182, 1); // pin 6 = down
    gpio_setup(48, 1); // pin 7 = right
    gpio_setup(49, 1); // pin 8 = up

    char buf[64];
    int len;

    while (1)
    {
        do {
            len = host_receive((unsigned char *)buf, 64);
            //mcu_sleep(10);
        } while (len <= 0);
        if (strcmp(buf, "") != 0) {
            /*
            *   buf = string of 4 bits ie, "0101"
            *   big endian, bit 0 = up, bit 1 = left, bit 2 = down, bit 3 = right
            *   1 = on, 0 = off
            */
            int up = (int)buf[0] - 48;
            int left = (int)buf[1] - 48;
            int down = (int)buf[2] - 48;
            int right = (int)buf[3] - 48;
            if(up == 1) {
                gpio_write(49, 1);
            }
            if (up == 0) {
                gpio_write(49, 0);
            }
            if(right == 1) {
                gpio_write(13, 1);
            }
            if(right == 0) {
                gpio_write(13, 0);
            }
            if(down == 1) {
                gpio_write(182, 1);
            }
            if(down == 0) {
                gpio_write(182, 0);
            }
            if(left == 1) {
                gpio_write(48, 1);
            }
            if(left == 0) {
                gpio_write(48, 0);
            }
        }
    }
}



