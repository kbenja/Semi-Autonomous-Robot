#include <iostream>
#include "../include/communication.h"
#include "../include/lidar_module.h"
#include "../include/i2c_library.h"

bool communication = false;
bool lidar_module = true;

int main(){
    std::cout << "Hello this is breakerbot" << std::endl;

    if(communication) {
        Communication input;
        while(1){
            std::string instruction = input.getInstruction();
            std::cout << "instruction: " << instruction << std::endl;
        }
    }

    if(lidar_module) {
        Lidar_Module l1(2);
        while(1) {
            std::cout << l1.get_distance_reading() << std::endl;
        }
    }

    return 0;
}
