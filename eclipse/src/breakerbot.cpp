#include <stdlib.h>
#include <cmath>
#include "include/ipc_module.h"
#include "include/test_module.h"
#include "include/swerve_module.h"
#include "include/pot_module.h"
#include "include/drive_module.h"
#include "include/intake_module.h"

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

bool main_program = true;
bool pot_testing = false;
bool stop_motors = false;
bool motor_testing = false;
bool navx_testing = false;
bool swerve_module = false;
bool drive_module = false;
bool drive_unit_testing = false;

int16_t instructions[2] = {-1,0};
int16_t *p_instructions = instructions;
int16_t sending[3] = {-1,-1,-2};
// int16_t *p_sending = sending;

int mode; // used to decide how to use robot
int input = -1;
int result; // used for error handling

int main(int argc, char** argv) {
    if (main_program) {
        float user_input = 0.0;                          // receive signal from argv or use default (stop)
        if (argc > 1) {
            printf("***Using given value by user\n");
            user_input = atof(argv[1]);
        }

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
        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t i2c_status = i2c_init_board(i2c, address); // initialize i2c board
        if (i2c_status != MRAA_SUCCESS) printf("[ !!! ] Can not initialize I2C Board.\n");

        Intake_Module i1 = Intake_Module(i2c, 8); // setup intake module on port 8

        Drive_Module d1 = Drive_Module(i2c); // initialize drive module
        while(1) {
            usleep(50000); // cycle time
            ipc.unix_socket_write(sending); // send most recent data to socket
            result = ipc.unix_socket_read(p_instructions); //result = 0 if empty socket, -1 if error, length if read
            // if(result > 0) {
            if(instructions[-2] != 0 && instructions[1] != -2) {
                mode = instructions[0];
                input = instructions[1];
            }
            switch(mode) {
                case -1: // server sends -1 of the client is disconnected
                    printf("CLIENT DISCONNECT STOPPING ALL MOTORS\n");
                    d1.stop(); // stop all motors
                    break;
                case 0: // Idle mode
                    if(instructions[1] != 0) {
                        // printf("Received OKAY command from intel edison %d\n", input);
                        printf("IDLE MODE, Waiting for input\n");
                    }
                    break;
                case 1: // Manual mode
                    switch(input) {
                        case 0: // break button pressed
                            // printf("Received BREAK command\n");
                            d1.stop();
                            break;
                        case 1: // forward button pressed
                            // printf("Received FORWARD command\n");
                            d1.drive('Y', user_input);
                            break;
                        case 2: // left button pressed
                            // printf("Received LEFT command\n");
                            d1.drive('X', user_input);
                            break;
                        case 3: // back button pressed
                            // printf("Received BACKWARDS command\n");
                            d1.drive('Y', -user_input);
                            break;
                        case 4: // right button pressed
                            // printf("Received RIGHT command\n");
                            d1.drive('X', -user_input);
                            break;
                        case 5: // right trigger button pressed
                            // printf("Received CLOCKWISE command\n");
                            d1.drive('Z', 0.3);
                            break;
                        case 6: // left trigger button pressed
                            // printf("Received COUNTER CLOCKWISE command\n");
                            d1.drive('Z', -0.3);
                            break;
                        default: // error has occured
                            break;
                    }
                    // wake up I2C chip so it registers commands
                    mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
                    break;
                case 2:
                    printf("AUTO MODE, INPUT = %d\n", input);
                    break;
                case 3:
                    printf("INTAKE MODE, INPUT = %d\n", input);
                    if(input == 1) {
                        printf("push out\n");
                        i1.drive_intake(0.5);
                    } else if (input == 3) {
                        printf("pull in\n");
                        i1.drive_intake(-0.55);
                    } else if (input == 0) {
                        i1.stop_intake();
                        printf("stop everything\n");
                    }
                    break;
                case 4:
                    printf("TESTING MODE, INPUT = %d\n", input);
                    break;
                default:
                    break;
            }
            sending[0] = x1.get_yaw();
            sending[1] = mode;
            sending[2] = input;
        }
    }

    if (drive_module) {
        printf("Testing drive module\n");
        int direction = 0;
        char axis = 'X';
        if (argc > 1) {
            printf("***Using given value by user\n");
            direction = atoi(argv[1]);
        }
        if (direction == 1) {
            axis = 'Y';
        } else if (direction == 2) {
            axis = 'Z';
        }
        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t i2c_status = i2c_init_board(i2c, address);
        if (i2c_status != MRAA_SUCCESS) printf("[ !!! ] Can not initialize I2C Board.\n");
        Drive_Module d1 = Drive_Module(i2c);
        while(1) {
            usleep(50000);
            d1.drive(axis, 0.7);
            mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00)); // wake up the board
            // sleep(2);
            // d1.stop();
            // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00)); // wake up the board
        }
    }

    if (drive_unit_testing) {
        int test = 0;
        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t i2c_status = i2c_init_board(i2c, address); // initialize i2c board
        if (i2c_status != MRAA_SUCCESS) printf("[ !!! ] Can not initialize I2C Board.\n");
        int test_result = 0;
        Drive_Module d1 = Drive_Module(i2c); // initialize drive module
        while(1) {
            usleep(50000); // cycle time
            switch(test) {
                case 0:
                    test_result = d1.drive('Y', 0.6);
                    // test_result = d1.drive('Y', 0.1);
                    break;
                case 1:
                    test_result = d1.drive('X', -0.6);
                    // test_result = d1.drive('X', 0.1);
                    break;
                case 2:
                    test_result = d1.drive('Y', -0.6);
                    // test_result = d1.drive('Y', 0.1);
                    break;
                case 3:
                    test_result = d1.drive('X', 0.5);
                    // test_result = d1.drive('X', 0.1);
                    break;
                case 4:
                    test_result = d1.drive('X', -0.8);
                    // test_result = d1.drive('X', 0.1);
                    break;
                default:
                    break;
            }
            if(test_result == 2) {
                printf("TEST %d COMPLETE\n", test);
                sleep(3);
                test_result = d1.stop();
                test++;
            }
            mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
        }
    }
    if (pot_testing) {
        int testing_port = 0;                          // default value of pot is 0
        if (argc > 1) {
            printf("***Using given value by user\n");
            testing_port = atoi(argv[1]);
        }
        test_pot_module(testing_port);
    }

    if (navx_testing) {
        test_navx_module();
    }

    if (stop_motors) {
        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t i2c_status = i2c_init_board(i2c, address);
        if (i2c_status != MRAA_SUCCESS) printf("[ !!! ] Can not initialize I2C Board.\n");
        Drive_Module d1 = Drive_Module(i2c);
        d1.stop();
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00)); // wake up the board
    }

    return 0;
}
