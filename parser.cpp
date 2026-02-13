#include "parser.h"
#include <sstream>

std::vector<std::string> Parser::parse(const std::string& input) {
    std::vector<std::string> v;
    std::stringstream ss(input);
    std::string s;

    while (getline(ss, s, ' ')) {
        v.push_back(s);
    }

    return v;
}