/*
 * Author: Jessica Gomez <jessica.gomez.hernandez@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mraa.h"

#include <stdio.h>
#include <unistd.h>

//Grey/Red: 	SDA		A4
//White/yellow:	SCL		A5


#define BOARD_ADDR 0x40	//board default address

#define ALL_LED_ON_H	0xFB	//all on high
#define ALL_LED_ON_L	0xFA	//all on low

/*
 * On board LED blink C example
 *
 * Demonstrate how to blink the on board LED, writing a digital value to an
 * output pin using the MRAA library.
 * No external hardware is needed.
 *
 * - digital out: on board LED
 *
 * Additional linker flags: none
 */

int main()
{
	mraa_init();	//initialize mraa
	mraa_i2c_context i2c = mraa_i2c_init(6);	//initialize I2C-6

	mraa_i2c_address(i2c, BOARD_ADDR);	//destination is board 0x40

	//construct I2C transmission buffer
	uint8_t transmission_buf[6];			//buffer
	transmission_buf[0] = ALL_LED_ON_H;		//register address
	transmission_buf[1] = 0x10;				//value (turn all LEDs on)

	mraa_i2c_write(i2c, transmission_buf, 2);	//write buffer to i2c bus
	return(0);
}
