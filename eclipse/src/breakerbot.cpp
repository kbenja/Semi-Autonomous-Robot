#include <stdlib.h>
#include "include/ipc_module.h"
#include "include/test_module.h"
#include "include/steering_module.h"
#include "include/swerve_module.h"
#include "include/pot_module.h"
#include "include/drive_module.h"
#include "include/lidar_module.h"
#include "include/alignment_module.h"
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

bool lidar_module = false;
bool alignment_module = false;

int16_t instructions[2] = {-1,0};
int16_t *p_instructions = instructions;
int16_t sending[3] = {-1,-1,-2};
// int16_t *p_sending = sending;

int mode; // used to decide how to use robot
int input = -1;
int result; // used for error handling

int main(int argc, char** argv) {

    if(lidar_module) {

    	printf("LIDAR MODULE TESTING\n\n");

        //INITIALIZATION
        Lidar_Module lidar(3); //ANALOG PIN #3
        while(1) {
            usleep(500000); //sleep every .5 sec
            printf("LIDAR:%d\n",lidar.get_average_distance_reading_int());
            //printf("LIDAR:%d\n",lidar.get_distance_reading_int());
            //printf("LIDAR:%f\n",lidar.get_distance_reading());

            //printf("LIDAR_AVERAGE:%f\n\n",lidar.get_average_distance_reading());

            //printf("LIDAR#1:%X - %d\n",l1.get_distance_reading());
            //printf("LIDAR#2:%X - %d\n\n",l2.get_distance_reading());
        }
    }

    if(alignment_module) {

        printf("ALIGNMENT MODULE TESTING\n\n");

        Alignment_Module lidar = Alignment_Module();

        while(1) {
            usleep(50000); //sleep every .5 sec

            //LIDAR
            lidar.lidar_alignment();

            //CAMERA
            //camera_alignment();

        }
    }


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
        bool stopping = false;
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
                case -1:
                    printf("CLIENT DISCONNECT STOPPING ALL MOTORS\n");
                    d1.stop(); // stop all motors
                    break;
                case 0:
                    if(instructions[1] != 0) {
                        // printf("Received OKAY command from intel edison %d\n", input);
                        printf("IDLE MODE, Waiting for input\n");
                    }
                    break;
                case 1:
                    if (input != 0) {
                        stopping = false;
                    }
                    switch(input) {
                        case 0:
                            printf("Received BREAK command\n");
                            if(!stopping) {
                                d1.stop();
                                stopping = true;
                            }
                            break;
                        case 1:
                            printf("Received FORWARD command\n");
                            d1.drive('Y', user_input);
                            // printf("User input: %f\n", user_input);
                            break;
                        case 2:
                            printf("Received LEFT command\n");
                            d1.drive('X', -user_input);
                            break;
                        case 3:
                            printf("Received BACKWARDS command\n");
                            d1.drive('Y', -user_input);
                            break;
                        case 4:
                            printf("Received RIGHT command\n");
                            d1.drive('X', user_input);
                            break;
                        case 5:
                            printf("Received CLOCKWISE command\n");
                            d1.drive('Z', user_input);
                            break;
                        case 6:
                            printf("Received COUNTER CLOCKWISE command\n");
                            d1.drive('Z', -user_input);
                            break;
                        default:
                            break;
                    }
                    mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
                    break;
                case 2:
                    printf("AUTO MODE, INPUT = %d\n", input);
                    break;
                case 3:
                    printf("INTAKE MODE, INPUT = %d\n", input);
                    if(input == 1) {
                        printf("push out\n");
                        i1.drive_intake(-0.5);
                    } else if (input == 3) {
                        printf("pull in\n");
                        i1.drive_intake(0.5);
                    } else if (input == 0) {
                        i1.stop_intake();
                        printf("stop everything\n");
                    }
                    break;
                case 4:
                    printf("TESTING MODE, INPUT = %d\n", input);
                    break;
                default:
                    printf("DEFAULT MODE\n");
                    break;
            }
            sending[0] = x1.get_yaw()/100;
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
        // Swerve_Module s1 = Swerve_Module(i2c, 1, 6, 7, 0, 0, 2508, 1992, 2127);
        // BR swerve M5
        // Swerve_Module s1 = Swerve_Module(i2c, 2, 4, 5, 1, 0, 1480, 1921, 1810);
        // BL swerve M1
        Swerve_Module s1 = Swerve_Module(i2c, 3, 0, 1, 2, 0, 2540, 2040, 2190);
        // FL swerve M3
        // Swerve_Module s1 = Swerve_Module(i2c, 4, 2, 3, 3, 0, 1753, 2266, 2089);


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
