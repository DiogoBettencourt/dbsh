#ifndef BUILTINS_H
#define BUILTINS_H

#include <vector>
#include "parser.h"

extern std::vector<std::string> builtinList;

bool isBuiltin(const Command& cmd);

void runBuiltin(const Command& cmd);

void dbsh_cd(const Command& cmd);

void dbsh_exit(const Command& cmd);

void dbsh_pwd(const Command& cmd);

void dbsh_echo(const Command& cmd);

void dbsh_export(const Command& cmd);

void dbsh_unset(const Command& cmd);

#endif
