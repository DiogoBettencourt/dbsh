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
        std::vector<Command> parsePipeline(const std::string& input);

    private:
        std::vector<std::string> tokenize(const std::string& input);
        Command buildCommand(const std::vector<std::string>& tokens);
};

#endif