#include "mraa.h"
#include <stdio.h>
#include <unistd.h>

#define BOARD_ADDR 0x40	//board default address = 0x40 = 0100_0000

#define ALL_LED_ON_L    ((uint8_t)0xFA)   	//all on low
#define ALL_LED_ON_H    ((uint8_t)0xFB)   	//all on high
#define ALL_ON_BYTE     ((uint8_t)0x10)   	//== 0001_0000
#define ZERO            ((uint8_t)0x00)   	//zero
#define ALL_ON_WORD     ((uint16_t)0x0010)

#define MODE1	((uint8_t)0x00)		//SLP, RST, AUTO_INC, EXT_CLK, ADDR
#define MODE2	((uint8_t)0x01)		//INV, OUT_CND, DRV_T
#define PRE_SCALE ((uint8_t)0xFE)	//PWM frequency scale
/*
	Mode 1:
	7	RESTART		1 = restart chip, 0 = nothing
	6	EXTCLK		1 = enable external clock
	5	AI			1 = enable auto-increment
	4	SLEEP		1 = sleep chip (oscillator off)
	|	 |
	3	SUB1
	2	SUB2
	1	SUB3		Sub addresses (IGNORE)
	0	ALLCALL		Response to all-call I2C address (IGNORE)
 */

/*
	STARTUP BEHAVIOR:
	MODE1: SLEEP + ALLCALL (0001_0001)
	MODE2: OPEN DRAIN


	Step 1: MODE1<-0x10 (0001_0000)		//disable all call, but keep asleep
	Step 2: MODE1<-0xA0	(1010_0000)		//enable restart, and enable auto-increment

 */
int main()
{
	mraa_i2c_context i2c;
	i2c = mraa_i2c_init(6);
	mraa_i2c_address(i2c, BOARD_ADDR);
	mraa_result_t result = MRAA_SUCCESS;

	/*
	printf("I2C write test\n");
	result = mraa_i2c_write_byte_data(i2c, 0x10, 0x07);
	if (result != MRAA_SUCCESS) {
		printf("    ERROR! Error code %d \n",result);
		return result;
	}
	*/

	printf("I2C read test\n");
	uint8_t data = mraa_i2c_read_byte_data(i2c, 4);
	printf("Register = %d \n", data);

	//LED blink
	while(1) {
		mraa_i2c_write_byte_data(i2c, ALL_ON_BYTE, ALL_LED_ON_H);
		sleep(1);
		mraa_i2c_write_byte_data(i2c, ZERO, ALL_LED_ON_H);
		sleep(1);
	}


	return MRAA_SUCCESS;

}
