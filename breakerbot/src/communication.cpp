#include <iostream>
#include <fstream>
#include "../include/communication.h"

Communication::Communication()
{

}

std::string Communication::getInstruction()
{
    std::ifstream infile("instructions.txt");

    std::string instruction;
    if(infile.is_open())
    {
        getline(infile, instruction);
        std::cout << instruction.data() << std::endl;
        infile.close();
    }
    else std::cout << "Cannot open file" << std::endl;

    return instruction.data();
}
