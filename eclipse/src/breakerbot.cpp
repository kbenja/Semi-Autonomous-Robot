#include <stdlib.h>
#include "include/ipc_module.h"
#include "include/test_module.h"
#include "include/steering_module.h"
#include "include/swerve_module.h"

enum modes {
    DISCONNECTED = -1,
    IDLE = 0,
    MANUAL = 1,
    AUTO = 2,
    INTAKE = 3,
    TESTING = 4
};

enum directions {
    BREAK = 0,
    FORWARD = 1,
    LEFT = 2,
    BACKWARD = 3,
    RIGHT = 4,
    ROTATE_CW = 5,
    ROTATE_CCW = 6
};

bool ipc_module = false;
bool swerve_module = true;

int16_t instructions[2] = {-1,0};
int16_t *p_instructions = instructions;
int16_t sending[3] = {-1,-1,-2};
int16_t *p_sending = sending;

int mode;
int input;
int result;

int main(int argc, char** argv) {
    // float user_input = 0.0;                          // receive signal from argv or use default (stop)
    // if (argc > 1) {
    //     printf("***Using given value by user\n");
    //     user_input = atof(argv[1]);
    // }

    if (ipc_module) {
        // UNIX SOCKET INITIALIZATION
        IPC_Module ipc("/tmp/breakerbot.socket");
        int status = ipc.unix_socket_initialize();
        while(status < 0) {
            status = ipc.unix_socket_initialize();
            printf("Cannot connect to UNIX socket, retrying\n");
            usleep(500000);
        }

        // INITIALIZE HARDWARE
        NavX_Module x1; // initialize NavX
        x1.set_zero(); // calibrate NavX

        uint8_t address = 0x40;                     // i2c chip is @ i2c address 0x40
        mraa_i2c_context i2c = mraa_i2c_init(6);    // create original context for i2c (bus 6)
        i2c_init_board(i2c, address);               // initialize the board (our i2c library function)

        //Swerve_Module s1 = Swerve_Module(i2c, 1, 1, 2, 1, 0); // ORIGINAL (i2c, id, direction_port, drive_port, pot_AI, optical_encoder_reg, SWERVE_position)

        while(1) {
            usleep(100000); // cycle time

            ipc.unix_socket_write(sending); // send most recent data to socket
            result = ipc.unix_socket_read(p_instructions); //result = 0 if empty socket, -1 if error, length if read
            if(result > 0) {
                mode = instructions[0];
                input = instructions[1];
            } else if (result < 0) {
                printf("UNIX SOCKET ERROR");
            }

            switch(mode) {
                case DISCONNECTED:
                    printf("CLIENT IS DISONNECTED\n");
                    break;
                case IDLE:
                    // printf("IDLE MODE, INPUT = %d\n", input);
                    break;
                case MANUAL:
                    printf("MANUAL MODE, INPUT = %d\n", input);
                    switch(instructions[1]) {
                        case BREAK:
                            break;
                        case FORWARD:
                            break;
                        case LEFT:
                            break;
                        case BACKWARD:
                            break;
                        case RIGHT:
                            break;
                        default:
                            break;
                    }
                    mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
                    break;
                case AUTO:
                    printf("AUTO MODE, INPUT = %d\n", input);
                    break;
                case INTAKE:
                    printf("INTAKE MODE, INPUT = %d\n", input);
                    break;
                case TESTING:
                    printf("TESTING MODE, INPUT = %d\n", input);
                    break;
                default:
                    printf("DEFAULT MODE\n");
                    break;
            }
            sending[0] = mode;
            sending[1] = input;
            sending[2] =  x1.get_yaw()/100;
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

        //Swerve_Module s1 = Swerve_Module(i2c, 1, 1, 2, 1, 0); // ORIGINAL

        //(i2c, id, direction_port, drive_port, pot_AI, optical_encoder_reg, SWERVE_position)

        Swerve_Module FR = Swerve_Module(i2c, 1, 1, 5, 1, 0, 1952, 2451, 2087); //Front-Right Wheel
        Swerve_Module BR = Swerve_Module(i2c, 2, 2, 6, 2, 0, 1877, 1392, 1733); //Back-Right Wheel
        Swerve_Module FL = Swerve_Module(i2c, 3, 3, 7, 3, 0, 2027, 1392, 1505); //Front-Left Wheel
        Swerve_Module BL = Swerve_Module(i2c, 4, 4, 8, 4, 0, 1994, 2488, 2139); //Front-Left Wheel

        while(1) {
            usleep(25000);
            //s1.rotate(1926); // ORIGINAL desired_pos Rotate to a specific ADC value, considering mapping ADC to Degree

            //Y_translation
            FR.rotate_position('Y');
            BR.rotate_position('Y');
            FL.rotate_position('Y');
            BL.rotate_position('Y');

            //X_translation
           	FR.rotate_position('X');
            BR.rotate_position('X');
            FL.rotate_position('X');
            BL.rotate_position('X');

            //Z_rotation
            FR.rotate_position('Z');
            BR.rotate_position('Z');
            FL.rotate_position('Z');
            BL.rotate_position('Z');

            mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));

        }

    }

    return 0;
}
