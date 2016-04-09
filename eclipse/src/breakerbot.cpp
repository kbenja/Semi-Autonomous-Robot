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
bool swerve_module = true;
bool pot_testing = false;

int16_t instructions[2] = {-1,0};
int16_t *p_instructions = instructions;
int16_t sending[3] = {-1,-1,-2};
int16_t *p_sending = sending;

int mode;
int input;
int result;

int main(int argc, char** argv) {
    if (ipc_module) {
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

        uint8_t address = 0x40;                     // i2c chip is @ i2c address 0x40
        mraa_i2c_context i2c = mraa_i2c_init(6);    // create original context for i2c (bus 6)
        mraa_result_t i2c_status = i2c_init_board(i2c, address);               // initialize the board (our i2c library function)
        if (i2c_status != MRAA_SUCCESS) {
            printf("[ !!! ] Can not initialize I2C Board.\n");
        }

        // ORIGINAL (i2c, id, direction_port, drive_port, pot_AI, optical_encoder_reg, SWERVE_position)
        Swerve_Module s1 = Swerve_Module(i2c, 1, 5, 1, 1, 0, 2000, 2000, 2000);
        Swerve_Module s2 = Swerve_Module(i2c, 1, 6, 2, 1, 0, 2000, 2000, 2000);
        Swerve_Module s3 = Swerve_Module(i2c, 1, 7, 3, 2, 0, 2000, 2000, 2000);
        Swerve_Module s4 = Swerve_Module(i2c, 1, 8, 4, 3, 0, 2000, 2000, 2000);


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
                                s1.stop_motors();
                                s2.stop_motors();
                                s3.stop_motors();
                                s4.stop_motors();
                                break;
                            case 1:
                                printf("Received FORWARD command\n");
                                s1.drive_wheel(user_input);
                                s2.drive_wheel(user_input);
                                s3.drive_wheel(user_input);
                                s4.drive_wheel(user_input);
                                break;
                            case 2:
                                printf("Received LEFT command\n");
                                break;
                            case 3:
                                printf("Received BACKWARDS command\n");
                                s1.drive_wheel(-user_input);
                                s2.drive_wheel(-user_input);
                                s3.drive_wheel(-user_input);
                                s4.drive_wheel(-user_input);
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
                        }
                        // wake up the board after sending an input
                        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
                        break;
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
            sending[0] = mode;
            sending[1] = input;
            sending[2] = x1.get_yaw()/100;
        }
    }
    if (swerve_module) {
        printf("Testing swerve module\n");


        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t i2c_status = i2c_init_board(i2c, address);
        if (i2c_status != MRAA_SUCCESS) printf("[ !!! ] Can not initialize I2C Board.\n");

        // dir port 1, drive port 2, Y, X
        Swerve_Module s2 = Swerve_Module(i2c, 1, 1, 2, 1, 0, 2467, 1955, 2235);
        while(1) {
            usleep(50000); //sleep for 0.05s
            s2.swerve_controller('X', 0.5, true); // try to rotate to the correct position
            usleep(50);
            mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00)); // wake up the board
        }
    }
    if (pot_testing) {
        int testing_port = 1;                          // default value of pot is 0
        if (argc > 1) {
            printf("***Using given value by user\n");
            testing_port = atoi(argv[1]);
        }
        Pot_Module pot = Pot_Module(testing_port, 12);
        while(1) {
            usleep(500000); // sleep for 0.5s
            printf("VALUE for port: %d equals %d\n", testing_port, pot.get_average_val());
        }
    }

    return 0;
}
