#include "shell.h"
#include "parser.h"
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

        std::vector<std::string> command = parser.parse(input);
    }
}