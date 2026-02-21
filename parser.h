#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

struct Command {
    std::string name;
    std::vector<std::string> args;
    std::string input_file;
    std::string output_file;
    bool background = false;
};

class Parser {
public:
    Command parse(const std::string& input);
};

#endif