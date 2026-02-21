#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "executor.h"
#include "parser.h"

TEST_CASE("Executor runs builtin command", "[executor][builtin]") {
    Command cmd;
    cmd.name = "cd";
    cmd.args = {"/tmp"};
    cmd.background = false;
    Executor executor;
    int result = executor.execute(cmd);
    REQUIRE(result == 0);
}

TEST_CASE("Executor runs external command", "[executor][external]") {
    Command cmd;
    cmd.name = "echo";
    cmd.args = {"Hello, World!"};
    cmd.background = false;
    Executor executor;
    int result = executor.execute(cmd);
    REQUIRE(result == 0);
}

