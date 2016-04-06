#include <stdlib.h>
#include "include/i2c_library.h"
#include "include/ipc_module.h"
#include "include/manual_control.h"
#include "include/lidar_module.h"
#include "include/motor_module.h"
#include "include/encoder_module.h"
#include "include/pot_module.h"
#include "include/navx_module.h"
#include "include/swerve_module.h"
#include "include/testing_module.h"

/*
* Instructions = { MODE, INPUT1 }
* MODE:
* -1 = disconnected
* 0 = idle mode
* 1 = manual input
*/
int mode = -1;
int16_t instructions[2] = {-1,0};
int16_t *p_instructions = instructions;

int main(int argc, char** argv) {
    float user_input = 0.0;                          // receive signal from argv or use default (stop)
    if (argc > 1) {
        printf("***Using given value by user\n");
        user_input = atof(argv[1]);
    }

    if (ipc_module) {
        /*
         *  SWERVE MODULE INITIALIZATION
         */
        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);    // get board context
        i2c_init_board(i2c, address);               // initialize the board

        // direction = port 1
        // driving = port 2
        // pot_adc = port 1
        // encoder = port 0
        Swerve_Module s1 = Swerve_Module(i2c, 1, 1, 2, 1, 0);

        /*
         * UNIX SOCKET INITIALIZATION
         */
        IPC_Module ipc("/tmp/breakerbot.socket");
        int status = ipc.unix_socket_initialize();
        if (status < 0) {
            printf("Cannot connect to socket\n");
        }
        // int heartbeat = 0; //make sure to communicate every 0.25 sec;
        while(1) {
            usleep(25000); // cycle time
            ipc.unix_socket_write(); // check for status .. eventually send status of devices
            ipc.unix_socket_read(p_instructions);
            mode = instructions[0];
            switch(mode) {
                case -1: // No user connected to web server
                    printf("CLIENT IS NOT CONNECTED\r");
                    break;
                case 0: // Idle mode, no recent commands
                    break;
                case 1:
                    printf("MODE = MANUAL CONTROL, INPUT = %d\n", instructions[1]);
                    switch(instructions[1]) {
                        case 1: //UP
                            s1.drive_forward(user_input);
                            break;
                        case 2: //LEFT
                            s1.rotate_cw();
                            break;
                        case 3: // DOWN
                            s1.drive_forward(-user_input);
                            break;
                        case 4: // RIGHT
                            // turn to X angle
                            s1.rotate_ccw();
                            break;
                        case 0:
                            s1.stop_motors();
                            break;
                        default:
                            break;
                    }
                    mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
                    break;
                default:
                    printf("CAUGHT IN DEFAULT\n");
                    break;
            }
        }
    }

    if (swerve_module) {
        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);    // get board context
        i2c_init_board(i2c, address);               // initialize the board
        // ID field
        // steering = I2C-PWM port 1
        // driving = I2C-PWM port 2
        // pot_adc = Analog port 1
        // encoder = I2C-PWM port 0
        Swerve_Module s1 = Swerve_Module(i2c, 1, 1, 2, 1, 0);

        //Swerve_Module FR = Swerve_Module(i2c, 1, 1, 5, 1, 0); //Front-Right Wheel
        //Swerve_Module BR = Swerve_Module(i2c, 2, 2, 6, 2, 0); //Back-Right Wheel
        //Swerve_Module FL = Swerve_Module(i2c, 3, 3, 7, 3, 0); //Front-Left Wheel
        //Swerve_Module BL = Swerve_Module(i2c, 4, 4, 8, 4, 0); //Front-Left Wheel

        while(1) {
            usleep(25000);
            s1.rotate(1926); //desired_pos Rotate to a specific ADC value, considering mapping ADC to Degree

            /*

            //Y_translation
            FR.Y_translation();
            BR.Y_translation();
            FL.Y_translation();
            BL.Y_translation();

            //X_translation
           	FR.X_right_translation();
            BR.X_right_translation();
            FL.X_left_translation();
            BL.X_left_translation();

            //Z_rotation
            FR.Z_rotation_FR();
            BR.Z_rotation_BR();
            FL.Z_rotation_FL();
            BL.Z_rotation_BL();

            */

            mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));

        }

    }

    return 0;
}
