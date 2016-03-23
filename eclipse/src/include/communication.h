#include <iostream>
#include <fstream>
#include <string>

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

/*
*
* Instructions by index:
* 0 = choose mode
* 1 = value for motor
*
* Different modes:
* -1 = disconnected
* 1 = idle
* 2 = reading values
*
*
*/

class Communication
{
public:
    Communication() {};

    int * get_instructions() {
        int length = 2;
        int temp;
        int index = 0;
        int * instructions;
        bool reading = true;
        instructions = new int[length];

        std::ifstream file;
        std::string instruction;
        file.open("../communication/to_breakerbot.txt");
        while(index < length && reading) {
            getline(file, instruction);
            if(index == 0 && std::stoi(instruction) == -1) {
                reading = false;
            }
            try {
                temp = std::stoi(instruction);
            } catch(const std::exception& e) {
                std::cout << "Exception catch" << std::endl;
                temp = -2;
            }
            *(instructions + index) = temp;
            index++;
        }
        file.close();
        delete[] instructions;
        return instructions;
    }
};

#endif
