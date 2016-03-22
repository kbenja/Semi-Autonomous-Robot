#include <iostream>
#include <fstream>

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Communication
{
public:
    Communication() {};
    std::string getInstruction() {
        std::ifstream file;
        std::string instruction;

        file.open("/dev/ttymcu0");
        getline(file, instruction);
        file.close();

        return instruction.data();
    }
};

#endif
