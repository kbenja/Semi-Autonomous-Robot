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
	3	SUB1
	2	SUB2
	1	SUB3		Sub addresses (IGNORE)
	0	ALLCALL		Response to all-call I2C address (IGNORE)
 */

int main()
{
	//system("sh ~/init_i2c6.sh");	//init i2c-6 bus
	//mraa_init();					//init mraa

	system("echo hello\n");

	mraa_result_t success;

	//initialize I2C in program
	mraa_i2c_context i2c = mraa_i2c_init(6);	//initialize I2C-6
	success = mraa_i2c_address(i2c, BOARD_ADDR);			//destination is address 0x40
	if (success != 0) {
		system("echo error\n");
		return(1);
	}
	//restart device (sleep, set mode and restart, wake)

	success = mraa_i2c_write_byte_data(i2c,((uint8_t)0x10), MODE1);	//0001_0000
	if (success != 0) {
		system("echo error\n");
		return(1);
	}
	success = mraa_i2c_write_byte_data(i2c,((uint8_t)0x30), MODE1);	//0011_0000
	if (success != 0) {
		system("echo error\n");
		return(1);
	}
	success = mraa_i2c_write_byte_data(i2c,((uint8_t)0x80), MODE1);	//1000_0000
	if (success != 0) {
		system("echo error\n");
		return(1);
	}

	//set frequency --> PRE_SCALE register
	//Ignore for now; worry when motors are used

	success = mraa_i2c_write_byte_data(i2c, ALL_ON_BYTE, ALL_LED_ON_H);
	if (success != 0) {
		system("echo error\n");
		return(1);
	}

}
