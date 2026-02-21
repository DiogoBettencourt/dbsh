#include "shell.h"
#include "executor.h"
#include <iostream>
#include <unistd.h>
#include <climits>

void Shell::run() {
    bool end = false;

    while (!end) {
        std::string input;
        
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << cwd << " $ ";
        } else {
            std::cout << "dbsh> ";
        }
        
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }   
        
        if (input == "exit") {
            end = true;
            continue;
        }

        Parser parser;
        Executor executor;

        if (input.find('|') != std::string::npos) {
            std::vector<Command> pipeline = parser.parsePipeline(input);
            executor.executePipeline(pipeline);
        } else {
            Command cmd = parser.parse(input);
            executor.execute(cmd);
        }
    }
}