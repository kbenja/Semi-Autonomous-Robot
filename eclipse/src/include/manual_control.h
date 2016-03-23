#include <iostream>
#include <fstream>

#ifndef MANUAL_CONTROL_H
#define MANUAL_CONTROL_H

class Manual_Control
{
public:
    Manual_Control() {};
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
