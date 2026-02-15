#include "builtins.h"

std::vector<std::string> builtinList = {"cd", "etc"};

bool isBuiltin(const Command& cmd) {
    for (std::string builtin : builtinList) {
        if (cmd.name == builtin) {
            return true;
        }
    }
    return false;
}