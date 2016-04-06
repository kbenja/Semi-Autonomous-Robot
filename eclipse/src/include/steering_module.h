#include <stdlib.h>
#include "mraa.h"
#include "swerve_module.h"

#ifndef STEERING_MODULE_H
#define STEERING_MODULE_H

// create 4 swerve modules
/*



INPUTS
Cartesian Direction
CW or CCW rotation
individual signals to motors
    send angle and velocity to individual motors on each module


OUTPUTS
values of pots
values of optical encoder

*/
/*

class Steering_Module {
public:
int id;
mraa_i2c_context i2c;
int[4] directions;
int[4] velocities;
Swerve_Module * front_right;
Swerve_Module * front_left;
Swerve_Module * rear_right;
Swerve_Module * rear_left;

Steering_Module() {
    printf("[ !!!! ] Do not use defualt constructor of steering module\n");
}

Steering_Module(int in_id, const mraa_i2c_context & in_i2c) {
    id = in_id;
    i2c = in_i2c;
    // const mraa_i2c_context & i2c_in, int module_id, int dir_port, int drive_port, int pot_adc, int encoder_port
    front_right = new Swerve_Module(in_i2c, in_id, 1, 2, 0, 0);
    front_left  = new Swerve_Module(in_i2c, in_id, 3, 4, 1, 0);
    rear_right  = new Swerve_Module(in_i2c, in_id, 5, 6, 2, 0);
    rear_left   = new Swerve_Module(in_i2c, in_id, 7, 8, 3, 0);
};

int drive() {
    // 0 if done turning, 1 if still turning, -1 if error
}

int stop() {
    // stop all motors
}

~Steering_Module() {
    printf("[ dest ] Steering Module %d deleted\n", id);
}
*/

#endif
