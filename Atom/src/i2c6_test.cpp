#include <iostream>
#include "mraa.h"
using namespace std;

#define LED0_ON_L	((uint8_t)0x06)
#define LED0_ON_H	((uint8_t)0x07)
#define LED0_OFF_L	((uint8_t)0x08)
#define LED0_OFF_H	((uint8_t)0x09)

#define ALWAYS_ON	((uint8_t)0x10)
#define ALWAYS_OFF	((uint8_t)0x00)


int main() {
  /* Setup your example here, code that should run once
   */


  /* Code in this loop will run repeatedly
   */

	system("echo hello\n");

	mraa_i2c_context i2c1;

	i2c1 = mraa_i2c_init(1);
	if (i2c1 == NULL) {
		system("echo 'I2C bus failed to initialize!'");
		return(1);
	}
	mraa_i2c_address(i2c1, ((uint8_t)0x40));

	mraa_result_t on = mraa_i2c_write_byte_data(i2c1, ALWAYS_ON, LED0_ON_H);
	mraa_result_t off = mraa_i2c_write_byte_data(i2c1, ALWAYS_OFF, LED0_OFF_H);


	return MRAA_SUCCESS;
}
