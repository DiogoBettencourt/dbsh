#include <string>
#include <vector>

struct Command {
    std::string name;
    std::vector<std::string> args;
    std::string input_file;
    std::string output_file;
};

class Parser {
public:
    Command parse(const std::string& input);
};