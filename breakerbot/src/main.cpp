#include "i2c_library.h"
#include "../include/communication.h"
#include "../include/lidar_module.h"
#include "../include/motor_module.h"

bool communication = false;
bool lidar_module = false;
bool motor_module = true;

int main(){
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

        mraa_i2c_context i2c = mraa_i2c_init(6);    // initialize the board
        mraa_i2c_address(i2c, 0x40);                // set board address

        printf("sleeping the board enable auto increment\n");
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x30), ((uint8_t) 0x00));

        printf("setting prescale value to 400Hz\n");
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x0e), ((uint8_t) 0xfe));

        printf("setting off value to 0\n");
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x00), ((uint8_t) 0xfd));

        printf("setting motors to specific values\n");

        Motor_Module m1(1);
        Motor_Module m2(2);
        Motor_Module m3(3);

        m1.send_signal(i2c, (float) 1.0);   // FORWARD (CCW) = 1.0
        m2.send_signal(i2c, (float) -1.0);  // REVERSE (CW) = -1.0
        m3.send_signal(i2c, (float) 0.0);   // STOP = 0.0

        printf("waking up the board\n");
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
    }

    return 0;
}
