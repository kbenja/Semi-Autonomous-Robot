#include <iostream>
#include "../include/communication.h"
#include "../include/i2c.h"

int main(){
    std::cout << "Hello this is breakerbot" << std::endl;

    Communication input;
    std::string instruction = input.getInstruction();

    std::cout << "instruction: " << instruction << std::endl;

    return 0;
}
