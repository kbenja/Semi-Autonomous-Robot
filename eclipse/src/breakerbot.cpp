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

int mode = 1; //idle by default

int main(int argc, char** argv) {

    if (ipc_module) {
        IPC_Module ipc("/tmp/breakerbot.socket");
        int status = ipc.unix_socket_initialize();
        if (status < 0) {
            printf("Cannot connect to socket\n");
        }
        while(1) {
            usleep(100000);
            ipc.unix_socket_read();
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

        float user_input = 0.0;                          // receive signal from argv or use default (stop)
        if (argc > 1) {
            printf("***Using given value by user\n");
            user_input = (int)atof(argv[1]);
        } else {
            printf("***Using default value of 0x0B00\n");
        }

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
