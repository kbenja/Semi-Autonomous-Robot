#include <stdlib.h>
#include "include/ipc_module.h"
#include "include/test_module.h"
#include "include/steering_module.h"
#include "include/swerve_module.h"
#include "include/pot_module.h"
#include "include/drive_module.h"

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
bool swerve_module = false;
bool pot_testing = false;
bool stop_motors = false;
bool motor_testing = false;
bool navx_testing = false;
bool drive_module = true;

int16_t instructions[2] = {-1,0};
int16_t *p_instructions = instructions;
int16_t sending[3] = {-1,-1,-2};
int16_t *p_sending = sending;

int mode;
int input;
int result;

int main(int argc, char** argv) {
    if (motor_testing) {
        float user_input = 0.0;                          // receive signal from argv or use default (stop)
        if (argc > 1) {
            printf("***Using given value by user\n");
            user_input = atof(argv[1]);
        }
        test_motor_module(user_input);
    }
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
        mraa_result_t i2c_status = i2c_init_board(i2c, address);               // initialize the board (our i2c library function)
        if (i2c_status != MRAA_SUCCESS) {
            printf("[ !!! ] Can not initialize I2C Board.\n");
        }

        while(1) {
            usleep(100000); // cycle time
            ipc.unix_socket_write(sending); // send most recent data to socket
            result = ipc.unix_socket_read(p_instructions); //result = 0 if empty socket, -1 if error, length if read
            if(result > 0) {
                mode = instructions[0];
                input = instructions[1];
                switch(mode) {
                    case -1:
                        printf("CLIENT IS DISONNECTED\r");
                        break;
                    case 0:
                        printf("IDLE MODE, INPUT = %d\n", input);
                        break;
                    case 1:
                        switch(instructions[1]) {
                            case 0:
                                printf("Received BREAK command\n");
                                break;
                            case 1:
                                printf("Received FORWARD command\n");
                                break;
                            case 2:
                                printf("Received LEFT command\n");
                                break;
                            case 3:
                                printf("Received BACKWARDS command\n");
                                break;
                            case 4:
                                printf("Received RIGHT command\n");
                                break;
                            case 5:
                                printf("Received CLOCKWISE command\n");
                                break;
                            case 6:
                                printf("Received COUNTER CLOCKWISE command\n");
                                break;
                            default:
                                break;
                        // wake up the board after sending an input
                        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
                        break;
                    }
                    case 2:
                        printf("AUTO MODE, INPUT = %d\n", input);
                        break;
                    case 3:
                        printf("INTAKE MODE, INPUT = %d\n", input);
                        break;
                    case 4:
                        printf("TESTING MODE, INPUT = %d\n", input);
                        break;
                    default:
                        printf("DEFAULT MODE\n");
                        break;
                }
            }
            // sending[0] = mode;
            sending[1] = input;
            sending[0] = x1.get_yaw()/100;
        }
    }

    if (drive_module) {
        printf("Testing drive module\n");
    }

    if (swerve_module) {
        printf("Testing swerve module\n");
        int swerve_result1;
        bool drive_proceed = false;

        int stop_point = 0;

        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t i2c_status = i2c_init_board(i2c, address);
        if (i2c_status != MRAA_SUCCESS) printf("[ !!! ] Can not initialize I2C Board.\n");


        // ID, dir port, drive port, AOIN, Encoder, Y, X, Z
        // FR swerve M7
        // Swerve_Module s1 = Swerve_Module(i2c, 1, 6, 7, 0, 0, 2437, 1925, 2075);
        // BR swerve M5
        Swerve_Module s1 = Swerve_Module(i2c, 2, 4, 5, 1, 0, 1484, 1952, 1828);
        // BL swerve M1
        // Swerve_Module s3 = Swerve_Module(i2c, 3, 0, 1, 2, 0, 2506, 2006, 2173);
        // FL swerve M3
        // Swerve_Module s4 = Swerve_Module(i2c, 4, 3, 2, 3, 0, 1662, 2175, 1981);


        while(1) {
            usleep(50000); //sleep for 0.05s
            switch (stop_point) {
                case 0:
                    swerve_result1 = s1.swerve_controller('X', 0.6, drive_proceed, false);
                    break;
                case 1:
                    swerve_result1 = s1.swerve_controller('Y', 0.6, drive_proceed, false);
                    break;
                case 2:
                    swerve_result1 = s1.swerve_controller('Z', 0.6, drive_proceed, false);
                    break;
                case 3:
                    swerve_result1 = s1.swerve_controller('Y', 0.6, drive_proceed, false);
                    break;
                case 4:
                    swerve_result1 = s1.swerve_controller('Y', 0.6, drive_proceed, false);
                    break;
                case 5:
                    swerve_result1 = s1.swerve_controller('X', 0.6, drive_proceed, false);
                    break;
                default:
                    printf("INCORRECT CASE\n");
                    break;
            }
            drive_proceed = false;
            if(swerve_result1 == 0) {
                stop_point++;
                stop_point = stop_point%5;
                printf("ALIGNED! moving to stopping point %d\n", stop_point);
                drive_proceed = true;
                swerve_result1 = s1.swerve_controller('X', 0.6, drive_proceed, true);
                usleep(1000000);
            }
            usleep(50);
            mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00)); // wake up the board
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
        while(1) {
            usleep(250000);
            test_navx_module();
        }
    }

    if (stop_motors) {
        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t i2c_status = i2c_init_board(i2c, address);
        if (i2c_status != MRAA_SUCCESS) printf("[ !!! ] Can not initialize I2C Board.\n");
        // FR swerve M7
        Swerve_Module s1 = Swerve_Module(i2c, 1, 7, 6, 0, 0, 2437, 1925, 2075);
        // BR swerve M5
        Swerve_Module s2 = Swerve_Module(i2c, 2, 5, 4, 1, 0, 1484, 1952, 1828);
        // BL swerve M1
        Swerve_Module s3 = Swerve_Module(i2c, 3, 1, 0, 2, 0, 2506, 2006, 2173);
        // FL swerve M3
        Swerve_Module s4 = Swerve_Module(i2c, 4, 3, 2, 3, 0, 1662, 2175, 1981);
        printf("Stopping all motors\n");
        s1.stop_motors();
        s2.stop_motors();
        s3.stop_motors();
        s4.stop_motors();
        sleep(1);
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00)); // wake up the board
    }

    return 0;
}
