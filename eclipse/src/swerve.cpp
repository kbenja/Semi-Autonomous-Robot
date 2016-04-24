#include <stdlib.h>
#include <cmath>
#include "include/ipc_module.h"
#include "include/test_module.h"
#include "include/swerve_module.h"
#include "include/pot_module.h"
#include "include/drive_module.h"

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

    printf("Testing swerve module\n");
    printf("argc = %d\n", argc);

    uint8_t address = 0x40;
    mraa_i2c_context i2c = mraa_i2c_init(6);
    mraa_result_t i2c_status = i2c_init_board(i2c, address);
    if (i2c_status != MRAA_SUCCESS) printf("[ !!! ] Can not initialize I2C Board.\n");

    int motor = 0; // default motor chosen
    char direction = 'X'; // default direction
    int PWM = 0;
    int swerve_port = 6;
    int drive_port = 7;

    Swerve_Module s1 = Swerve_Module(i2c, 0, swerve_port, drive_port, motor, 0, 2508, 1992, 2127);
    if (argc > 1) {
        motor = atoi(argv[1]);
        if (atoi(argv[1]) == 0) {
            printf("FRONT RIGHT motor chosen, %s\n", argv[1]);
            swerve_port = 6;
            drive_port = 7;
        } else if (atoi(argv[1]) == 1) {
            printf("BACK RIGHT motor chosen, %s\n", argv[1]);
            swerve_port = 4;
            drive_port = 5;
        } else if (atoi(argv[1]) == 2) {
            printf("BACK LEFT motor chosen, %s\n", argv[1]);
            swerve_port = 0;
            drive_port = 1;
        } else if (atoi(argv[1]) == 3) {
            printf("FRONT LEFT motor chosen, %s\n", argv[1]);
            swerve_port = 2;
            drive_port = 3;
        }
    }
    if (argc > 3) {
        printf("CUSTOM direction: %d\n", atoi(argv[3]));
        direction = 'X';
        PWM = atoi(argv[3]);
    } else {
        if (argc > 2) {
            direction = argv[2][0];
            if (argv[2][0] == 'X') {
                printf("X direction chosen\n");
            } else if (argv[2][0] == 'Y') {
                printf("Y direction chosen\n");
            } else if (argv[2][0] == 'Z') {
                printf("Z direction chosen\n");
            }
        }
    }
    printf("MOTOR: %d, DIRECTION %c\n", motor, direction);
    if (PWM != 0) {
        Swerve_Module s1 = Swerve_Module(i2c, 0, swerve_port, drive_port, motor, 0, PWM, PWM, PWM);
        printf("GOING TO CUSTOM VALUE OF %d\n", PWM);
    } else {
        if(motor == 0) {
            Swerve_Module s1 = Swerve_Module(i2c, 1, swerve_port, drive_port, 0, 0, 2508, 1992, 2127);
        } else if (motor == 1) {
            Swerve_Module s1 = Swerve_Module(i2c, 2, swerve_port, drive_port, 1, 0, 1480, 1921, 1810);
        } else if (motor == 2) {
            Swerve_Module s1 = Swerve_Module(i2c, 3, swerve_port, drive_port, 2, 0, 2540, 2040, 2190);
        } else if (motor == 3) {
            Swerve_Module s1 = Swerve_Module(i2c, 4, swerve_port, drive_port, 3, 0, 1753, 2266, 2089);
        }
    }
    bool waiting = false;
    int swerve_result1;
    while(1) {
        usleep(50000); //sleep for 0.05s
        swerve_result1 = s1.swerve_controller(direction, 0.6, false, waiting);
        if(swerve_result1 == 0) {
            printf("ALIGNED!\n");
            waiting = true;
            swerve_result1 = s1.swerve_controller(direction, 0.6, false, waiting );
        }
        usleep(50);
        mraa_i2c_write_byte_data(i2c, ((uint8_t) 0xa0), ((uint8_t) 0x00)); // wake up the board

    }

    return 0;
}
