#include <stdlib.h>
#include "../include/i2c_library.h"
#include "../include/communication.h"
#include "../include/lidar_module.h"
#include "../include/motor_module.h"

bool communication = false;
bool lidar_module = false;
bool motor_module = true;

int main(int argc, char** argv) {
    if(communication) {
        printf("COMMUNICATION MODULE TESTING\n\n");

        Communication input;
        while(1){
            std::string instruction = input.getInstruction();
            printf("instruction: %s\n", instruction.c_str());
        }
    }

    if(lidar_module) {
        printf("LIDAR MODULE TESTING\n\n");

        Lidar_Module l1(2);
        while(1) {
            printf("%f\n",l1.get_distance_reading());
        }
    }

    if(motor_module) {
        printf("MOTOR MODULE TESTING\n\n");
        uint8_t address = 0x40;
        mraa_i2c_context i2c = mraa_i2c_init(6);    // get board context
        i2c_init_board(i2c, address);               // initialize the board

        Motor_Module m1(1);                         // create motors to ports 1, 2, and 3
        Motor_Module m2(2);
        Motor_Module m3(3);

        double_reg signal;                          // receive signal from argv or use default (stop)
        if (argc > 1 && (int)atof(argv[1]) != 0 && (int)atof(argv[1]) < 0x10000) {
            printf("***Using given value by user\n");
            signal.u_sixteen = (int)atof(argv[1]);
        } else {
            printf("***Using default value of 0x0B00\n");
            signal.u_sixteen =  0x0B00;
        }

        m1.send_signal(i2c, signal);                // send signal to boards
        m2.send_signal(i2c, signal);
        m3.send_signal(i2c, signal);

        printf("waking up the board\n");
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
    }

    return 0;
}
