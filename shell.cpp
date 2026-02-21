#include "shell.h"
#include "executor.h"
#include <iostream>

void Shell::run() {
    bool end = false;

    while (!end) {
        std::string input;
        std::cout << "dbsh> ";
        std::getline(std::cin, input);
        
        if (input == "exit") {
            end = true;
        }

        Parser parser;
        Command cmd = parser.parse(input);
        Executor executor;
        executor.execute(cmd);
    }
}