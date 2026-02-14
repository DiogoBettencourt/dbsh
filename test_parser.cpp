#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "parser.h"

TEST_CASE("Parser splits input into command and args", "[parser]") {
    Parser parser;
    auto cmd = parser.parse("ls -l /home");
    REQUIRE(cmd.name == "ls");
    REQUIRE(cmd.args.size() == 2);
    REQUIRE(cmd.args[0] == "-l");
    REQUIRE(cmd.args[1] == "/home");
}

TEST_CASE("Parser handles empty input", "[parser]") {
    Parser parser;
    auto cmd = parser.parse("");
    REQUIRE(cmd.name == "");
    REQUIRE(cmd.args.empty());
}
