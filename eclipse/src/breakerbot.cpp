#include <stdlib.h>
#include "include/i2c_library.h"
#include "include/ipc_module.h"
#include "include/manual_control.h"
#include "include/lidar_module.h"
#include "include/motor_module.h"
#include "include/encoder_module.h"
#include "include/pot_module.h"
#include "include/navx_module.h"

bool lidar_module = false;
bool motor_module = false;
bool ipc_module = true;
bool encoder_module = false;
bool pot_module = false;
bool navx_module = false;
bool manual_control = false;

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
int16_t sending[2] = {-1,0};
int16_t *p_sending = sending;

int main(int argc, char** argv) {
    float user_input = 0.0;                          // receive signal from argv or use default (stop)
    if (argc > 1) {
        printf("***Using given value by user\n");
        user_input = atof(argv[1]);
    }

    if (ipc_module) {
        /*
         * NAVX MODULE INITIALIZATION
         */
        NavX_Module x1;
        x1.set_zero();
        /*
         *  MOTOR MODULE INITIALIZATION
         */
        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);    // get board context
        i2c_init_board(i2c, address);               // initialize the board

        Motor_Module m1(1);                         // create motors to ports 1, 2, and 3
        Motor_Module m2(2);

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
            usleep(100000); // cycle time
            sending[0] =  x1.get_yaw()/100;
            ipc.unix_socket_write(sending);
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
                    if(instructions[1] == 1) {
                        m1.send_signal(i2c, user_input);
                        m2.send_signal(i2c, user_input);
                    } else if (instructions[1] == 3) {
                        m1.send_signal(i2c, -user_input);
                        m2.send_signal(i2c, -user_input);
                    } else if (instructions[1] == 2) {
                        user_input += 0.1;
                        printf("Speed = %f\n",user_input);
                    } else if (instructions[1] == 4) {
                        user_input -= 0.1;
                        printf("Speed = %f\n",user_input);
                    } else if (instructions[1] == 0) {
                        m1.send_signal(i2c, 0);
                        m2.send_signal(i2c, 0);
                    }
                    mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
                    break;
                default:
                    printf("CAUGHT IN DEFAULT\n");
                    break;
            }
        }
    }

    if (pot_module) {
        printf("Pot module testing");
        Pot_Module P0(0,12);
        Pot_Module P1(1,12);

        while(pot_module) {
            printf("Pot 1: %d     Pot 2: %d\n",P0.get_val(),P1.get_val());
            usleep(500000);
        }
    }

    if (manual_control) {
        printf("MANUAL CONTROL MODULE TESTING\n\n");

        Manual_Control input;
        while(1){
            std::string instruction = input.getInstruction();
            printf("instruction: %s\n", instruction.c_str());
        }
    }

    if (lidar_module) {
        printf("LIDAR MODULE TESTING\n\n");

        Lidar_Module l1(2);
        while(1) {
            printf("%f\n",l1.get_distance_reading());
        }
    }

    if (motor_module) {
        printf("MOTOR MODULE TESTING\n\n");

        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);    // get board context
        i2c_init_board(i2c, address);               // initialize the board

        Motor_Module m1(1);                         // create motors to ports 1, 2, and 3
        Motor_Module m2(2);
        Motor_Module m3(3);
        Motor_Module m4(4);

        m1.send_signal(i2c, user_input);                // send signal to boards
        m2.send_signal(i2c, user_input);
        m3.send_signal(i2c, user_input);
        m4.send_signal(i2c, user_input);

        printf("waking up the board\n");
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
    }

    if (encoder_module) {
        sleep(1);
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t result = MRAA_SUCCESS;
        result = mraa_i2c_address(i2c, 0x30);
        if (result != MRAA_SUCCESS) {
            printf("was not able to connect to address\n");
        } else {
            printf("Was able to write to the device\n");
        }
        Encoder_Module e1(1);
        uint8_t higher_bits = mraa_i2c_read_byte_data(i2c, 0x44);
        uint8_t lower_bits = mraa_i2c_read_byte_data(i2c, 0x45);
        if (result != MRAA_SUCCESS) {
            printf("Cannot read from byte\n");
        } else {
            printf("Reading 0x44 = 0x%02x  0x45 = 0x%02x\n", higher_bits, lower_bits);
        }
    }

    if (navx_module) {
        int count = 0;
        printf("Creating NavX module\n");
        NavX_Module x1;
        while(1) {
            usleep(500000); //sleep for 1/2
            printf("Getting NavX fused heading value\n");
            printf("Value read = %d\n", x1.get_yaw()/100);
            count++;
            if (count%10 == 0) {
                x1.set_zero();
                printf("\n[ !!!! ] Setting zero on NavX\n");
            }
        }
    }

    return 0;
}
