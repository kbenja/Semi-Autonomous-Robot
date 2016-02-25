#include "mraa.h"
#include <stdio.h>
#include <unistd.h>

//Grey/Red: 	SDA		A4
//White/yellow:	SCL		A5

#define BOARD_ADDR 0x40	//board default address = 0x40 = 0100_0000

#define ALL_LED_ON_L    ((const uint8_t)0xFA)   //all on low
#define ALL_LED_ON_H    ((const uint8_t)0xFB)   //all on high
#define ALL_ON_BYTE     ((const uint8_t)0x10)   //== 0001_0000
#define ZERO            ((const uint8_t)0x00)   //zero
#define ALL_ON_WORD     ((const uint16_6)0x0010)

int main()
{    
	mraa_init();	//initialize mraa

    //initialize I2C
	mraa_i2c_context i2c = mraa_i2c_init(6);	//initialize I2C-6
	mraa_i2c_address(i2c, BOARD_ADDR);	        //destination is board 0x40

    //address + 1 byte data
    /*
        SKIP; NOT POSSIBLE DUE TO I2C_ADDR + REG_ADDR + DATA
        CONVENTION USED BY PCA9685 (WOULD SEND ADDR + DATA REPEATEDLY).
    */

    //address + register + 1 byte Data
    mraa_i2c_write_byte_data(i2c, ALL_ON_BYTE, ALL_LED_ON_H);   //i2c object, 8b, register

    //address + register + 2 bytes data
    //write LO, HI due to register numbering on PCA9685
    mraa_i2c_write_word_data(i2c, ALL_ON_WORD, ALL_LED_ON_L);   //i2c object, 16b data, register

    //address + variable-length data (2 bytes = register + data)
    unit8_t buf[2];
    buf[0] = ALL_LED_ON_H;  //register  0xFB
    buf[1] = ALL_ON_BYTE;   //value     0x10
    mraa_i2c_write(i2c, buf, 2);    //i2c object, data buffer, buffer length

    
	return(0);
}
