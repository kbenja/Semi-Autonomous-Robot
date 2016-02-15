#include "mcu_api.h"
#include "mcu_errno.h"
#include "PCA9685.h"




void mcu_main()
{
    /* your configuration code starts here */

	int confirm = i2c_write(PCA9685_ADDR, LED0_ON_L, ,1);

    while (1)       /* your loop code starts here */
    {
    }
}
