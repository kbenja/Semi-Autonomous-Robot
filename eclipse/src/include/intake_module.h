#include "motor_module.h"

#ifndef INTAKE_MODULE_H
#define INTAKE_MODULE_H
class Intake_Module {
public:
    mraa_i2c_context i2c_context;
    Motor_Module * intake_motor;
    int intake_port;

    /*
        Create Intake Module
        @param  mraa_i2c_context i2c:   i2c context setup in main function
        @param  int              port:  port to set motor module at
     */
    Intake_Module(const mraa_i2c_context & i2c, int port) {
        printf("[ init ] initialized intake motor on port %d\n", port);
        intake_port = port;
        intake_motor = new Motor_Module(intake_port);
        i2c_context = i2c;
    }

    /*
        Rotates drive wheel forwards
    */
    int drive_intake(float speed) {
        mraa_result_t result = intake_motor->send_signal(i2c_context, speed);
        return (result != MRAA_SUCCESS ? -1 : 0);
    }

    int stop_intake() {
        mraa_result_t result = intake_motor->send_signal(i2c_context, 0);
        return (result != MRAA_SUCCESS ? -1 : 0);
    }
};

#endif
