#include "parser.h"
#include <sstream>

std::vector<std::string> Parser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (getline(ss, token, ' ')) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

Command Parser::buildCommand(const std::vector<std::string>& tokens) {
    Command cmd;
    if (tokens.empty()) {
        return cmd;
    }

    cmd.name = tokens[0];
    for (size_t i = 1; i < tokens.size(); i++) {
        const std::string& token = tokens[i];
        if (token == "<" && i + 1 < tokens.size()) {
            cmd.input_file = tokens[++i];
        } else if (token == ">" && i + 1 < tokens.size()) {
            cmd.output_file = tokens[++i];
        } else if (token == "&") {
            cmd.background = true;
        } else {
            cmd.args.push_back(token);
        }
    }
    return cmd;
}

Command Parser::parse(const std::string& input) {
    std::vector<std::string> tokens = tokenize(input);
    return buildCommand(tokens);
}

std::vector<Command> Parser::parsePipeline(const std::string& input) {
    std::vector<Command> pipeline;
    std::vector<std::string> commandStrings;
    std::string currentCommand;

    for (char c : input) {
        if (c == '|') {
            if (!currentCommand.empty()) {
                commandStrings.push_back(currentCommand);
                currentCommand.clear();
            }
        } else {
            currentCommand += c;
        }
    }
    if (!currentCommand.empty()) {
        commandStrings.push_back(currentCommand);
    }

    for (const std::string& cmdStr : commandStrings) {
        std::vector<std::string> tokens = tokenize(cmdStr);
        if (!tokens.empty()) {
            Command cmd = buildCommand(tokens);
            pipeline.push_back(cmd);
        }
    }
    return pipeline;
}