#ifndef BUILTINS_H
#define BUILTINS_H

#include <vector>
#include "parser.h"

extern std::vector<std::string> builtinList;

bool isBuiltin(const Command& cmd);

void runBuiltin(const Command& cmd);

#endif
