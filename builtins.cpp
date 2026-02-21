#include "builtins.h"
#include <iostream>

std::vector<std::string> builtinList = {"cd", "etc"};

bool isBuiltin(const Command& cmd) {
    for (std::string builtin : builtinList) {
        if (cmd.name == builtin) {
            return true;
        }
    }
    return false;
}

void runBuiltin(const Command& cmd) {
    std::cout << "Running builtin command: " << cmd.name << std::endl;
}