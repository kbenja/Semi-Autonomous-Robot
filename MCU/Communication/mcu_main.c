#include "mcu_api.h"
#include "mcu_errno.h"

void mcu_main()
{
    /* your configuration code starts here */
    char buf[5];
    int len;
    char code[5] = "0000\n";
    while (1)
    {
        do {
            len = host_receive((unsigned char *)buf, 6);
            //mcu_sleep(10);
        } while (len <= 0);
        if (strcmp(buf, "") != 0) {
            code[0] = buf[0];
            code[1] = buf[1];
            code[2] = buf[2];
            code[3] = buf[3];
            // "0101"

            //debug_print(DBG_INFO, code);
            host_send((unsigned char*)code, 5);
        }
    }
}
