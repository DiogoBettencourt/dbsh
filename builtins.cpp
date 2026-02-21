#include "builtins.h"
#include <iostream>
#include <unistd.h>
#include <climits>

std::vector<std::string> builtinList = {"cd", "exit", "pwd", "echo", "export", "unset"};

bool isBuiltin(const Command& cmd) {
    for (std::string builtin : builtinList) {
        if (cmd.name == builtin) {
            return true;
        }
    }
    return false;
}

void runBuiltin(const Command& cmd) {
    if (cmd.name == "cd") {
        dbsh_cd(cmd);
    } else if (cmd.name == "exit") {
        dbsh_exit(cmd);
    } else if (cmd.name == "pwd") {
        dbsh_pwd(cmd);
    } else if (cmd.name == "echo") {
        dbsh_echo(cmd);
    } else if (cmd.name == "export") {
        dbsh_export(cmd);
    } else if (cmd.name == "unset") {
        dbsh_unset(cmd);
    }
}    

void dbsh_cd(const Command& cmd) {
    const char* path = cmd.args.empty() ? getenv("HOME") : cmd.args[0].c_str();
    
    if (chdir(path) != 0) {
        perror("cd");
        return;
    }
    
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        setenv("PWD", cwd, 1);
    }
}

void dbsh_exit(const Command& cmd) {
    exit(0);
}

void dbsh_pwd(const Command& cmd) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << std::endl;
    } else {
        perror("pwd");
    }
}

void dbsh_echo(const Command& cmd) {
    for (size_t i = 0; i < cmd.args.size(); i++) {
        std::cout << cmd.args[i];
        if (i < cmd.args.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void dbsh_export(const Command& cmd) {
    if (cmd.args.empty()) {
        std::cerr << "export: usage: export VAR=value" << std::endl;
        return;
    }
    
    for (const auto& arg : cmd.args) {
        size_t pos = arg.find('=');
        if (pos != std::string::npos) {
            std::string var = arg.substr(0, pos);
            std::string value = arg.substr(pos + 1);
            setenv(var.c_str(), value.c_str(), 1);
        } else {
            std::cerr << "export: invalid format: " << arg << std::endl;
        }
    }
}

void dbsh_unset(const Command& cmd) {
    if (cmd.args.empty()) {
        std::cerr << "unset: usage: unset VAR" << std::endl;
        return;
    }
    
    for (const auto& var : cmd.args) {
        unsetenv(var.c_str());
    }
}