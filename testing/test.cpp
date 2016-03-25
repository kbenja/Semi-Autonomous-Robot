#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::vector<int> &split(const std::string &s, char delim, std::vector<int> &elems) {
    std::stringstream ss(s);
    std::string item;
    int temp;
    while (std::getline(ss, item, delim)) {
        ss >> temp;
        if(ss.fail()) {
            elems.push_back(-1);
        } else {
            elems.push_back(temp);
        }
    }
    return elems;
}

std::vector<int> split(const std::string &s, char delim) {
    std::vector<int> elems;
    split(s, delim, elems);
    return elems;
}

int main(int argc, char** argv) {
    std::string instruction;
    bool reading = true;
    // instruction needs to start with a space, and all entries need to be integers.
    if (argc > 1) {
        instruction = argv[1];
    } else {
        std::ifstream file;
        file.open("./testing/test.txt");
        getline(file, instruction);
        file.close();
    }
    std::cout << "Instruction read: " << instruction << std::endl;
    // split read string into integer vector
    std::vector<int> instructions;
    instructions = split(instruction, ' ');

    // read back the contents of the vector
    for (std::vector<int>::iterator it = instructions.begin(); it != instructions.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << "\nSize: " << instructions.size() << '\n';
    return 0;
}
