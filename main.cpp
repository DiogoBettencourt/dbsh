#include <iostream>
#include <string>


int main() {
    bool end = false;

    std::string validCommands[] = {"exit", "help", "clear"};

    while (!end) {
        std::string input;
        std::cout << "dbsh> ";
        std::getline(std::cin, input);
        for (const std::string& command : validCommands) {
            if (input == "exit") {
                end = true;
                break;
            }else if (command == input) {
                //run command
            } else {
                std::cout << "Invalid Command" << std::endl;
                break;
            }
        }
    }   
}