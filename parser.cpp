#include "parser.h"
#include <sstream>

Command Parser::parse(const std::string& input) {
    Command cmd;
    std::vector<std::string> v;
    std::stringstream ss(input);
    std::string s;

    while (getline(ss, s, ' ')) {
        if (s.empty()) {
            continue;
        }
        v.push_back(s);
    }

    if (!v.empty()) {
        cmd.name = v[0];
        if (v.size() > 1)
            cmd.args = std::vector<std::string>(v.begin() + 1, v.end());
        
        if (!cmd.args.empty() && cmd.args.back() == "&") {
            cmd.background = true;
            cmd.args.pop_back();
        }
    }
    return cmd;
}