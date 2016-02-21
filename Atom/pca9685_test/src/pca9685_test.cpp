#include <unistd.h>
#include <signal.h>
#include <iostream>
#include "pca9685.h"

using namespace std;	//urgh

int main(int argc, char **argv) {

	//instantiate leds as a PCA9685 object
	upm::PCA9685 *leds = new upm::PCA9685(PCA9685_I2C_BUS, PCA9685_DEFAULT_I2C_ADDR);

	leds->setModeSleep(true);		//sleep PCA9685
	leds->setPrescaleFromHz(50);	//set 50Hz scale
	leds->setModeSleep(false);		//wake up

	leds->ledOnTime(PCA9685_ALL_LED, 0);	//turn all LEDs on
	leds->ledOffTime(PCA9685_ALL_LED, 2048);	//turn all LEDs off exactly halfway (50% duty cycle

	sleep(5);

	delete leds;
	return 0;
}
