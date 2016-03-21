#include <stdlib.h>
#include "../include/i2c_library.h"
#include "../include/communication.h"
#include "../include/lidar_module.h"
#include "../include/motor_module.h"
#include "../include/encoder_module.h"
// #include "../include/navx_module.h"


bool communication = false;
bool lidar_module = false;
bool motor_module = false;
bool encoder_module = false;
bool navx_module = true;

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

        Motor_Module m1(2);                         // create motors to ports 1, 2, and 3
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
        mraa_i2c_context i2c = mraa_i2c_init(6);
        mraa_result_t result = MRAA_SUCCESS;
        result = mraa_i2c_address(i2c, 0x32);
        if(result != MRAA_SUCCESS){
            printf("was not able to connect to address\n");
        } else {
            printf("Was able to write to the device\n");
        }
        uint8_t high_bits;
        uint8_t low_bits;
        double_reg combined;
        int status = 1;
        while(1) {
            usleep(500000); //sleep for 1/2
            status = mraa_i2c_write_byte(i2c, 0x16);
            status = mraa_i2c_read(i2c, &high_bits, 1);

            status = mraa_i2c_write_byte(i2c, 0x17);
            status = mraa_i2c_read(i2c, &low_bits, 1);
            if(status != 1) {
                printf("Could not read / write from the navx board\n");
            }

            combined.upper = high_bits;
            combined.lower = low_bits;

            float final = (float)combined.u_sixteen/100.0;
            printf("Yaw: High: 0x%04x, Low: 0x%04x\n", high_bits, low_bits);
            printf("Combined value = 0x%04x, decimal = %f\n", combined.u_sixteen, final);
            final = (final / 655.35) * 360;
            printf("\nFINAL value = %f\n\n", final);
        }
    }

    return 0;
}
