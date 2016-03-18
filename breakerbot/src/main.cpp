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
        // testing different values

        // CCW medium speed
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xff), ((uint8_t) 0xfc));
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x07), ((uint8_t) 0xfd));

        // CCW alittle bit faster
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xff), ((uint8_t) 0xfc));
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x05), ((uint8_t) 0xfd));

        // CCW slower than before
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x99), ((uint8_t) 0xfc));
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x09), ((uint8_t) 0xfd));

        // CW medium speed
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xcc), ((uint8_t) 0xfc));
        // mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x0c), ((uint8_t) 0xfd));

        // CCW slower speed 0x0800
        // CCW medium speed 0x0A00
        // CCW faster speed 0x0F00

        // STOP = 0x0B00
        // STOP = 0x0400
        //
        // FORWARD (CCW) = 0x0777
        // REVERSE (CW) = 0x0F00

        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x0B), ((uint8_t) 0xfd));
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0x00), ((uint8_t) 0xfc));


        // Motor_Module m1(0x06);

        // union double_reg signal;
        // signal.u_sixteen = 0xcc0c;
        // m1.send_signal(i2c, signal);

        std::cout << "waking up the board" << std::endl;
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00));

    }

    return 0;
}
