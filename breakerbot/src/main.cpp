#include <iostream>
#include "../include/communication.h"
#include "../include/lidar_module.h"
#include "../include/motor_module.h"

bool communication = false;
bool lidar_module = false;
bool motor_module = true;

int main(){
    if(communication) {
        std::cout << "COMMUNICATION MODULE TESTING\n\n" << std::endl;

        Communication input;
        while(1){
            std::string instruction = input.getInstruction();
            std::cout << "instruction: " << instruction << std::endl;
        }
    }

    if(lidar_module) {
        std::cout << "LIDAR MODULE TESTING\n\n" << std::endl;

        Lidar_Module l1(2);
        while(1) {
            std::cout << l1.get_distance_reading() << std::endl;
        }
    }

    if(motor_module) {
        std::cout << "MOTOR MODULE TESTING\n\n" << std::endl;

        // test talking to the board

        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_i2c_address(i2c, 0x40);

        //initialize the board
        std::cout << "sleeping the board enable auto increment" << std::endl;
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x30), ((uint8_t) 0x00));

        std::cout << "setting prescale value to 400Hz" << std::endl;
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x0e), ((uint8_t) 0xfe));

        std::cout << "setting off value to 0" << std::endl;
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x00), ((uint8_t) 0xfd));

        std::cout << "setting value to all PWM outputs" << std::endl;

        // STOP = 0x0B00
        // STOP = 0x0400
        //
        // FORWARD (CCW) = 0x0777
        // REVERSE (CW) = 0x0F00
        //
        // working code here
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x07), ((uint8_t) 0xfd));
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x77), ((uint8_t) 0xfc));

        // create motor module class and run via the class method:

        Motor_Module m1(2);
        Motor_Module m2(3);
        union double_reg signal;

        // high bits
        signal.u_eight[0]= 0x0F;
        // low bits
        signal.u_eight[1] = 0x00;
        m1.send_signal(i2c, signal);
        m2.send_signal(i2c, signal);

        // // writing to port 2
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x07), ((uint8_t) 0x11));
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x77), ((uint8_t) 0x10));

        // // writing to port 3
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x0F), ((uint8_t) 0x15));
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x00), ((uint8_t) 0x14));


        std::cout << "waking up the board" << std::endl;
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));
    }

    return 0;
}
