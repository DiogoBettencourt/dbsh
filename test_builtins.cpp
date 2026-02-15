#define Catch_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "builtins.h"

TEST_CASE("isBuiltin identifies built-in commands", "[builtins]") {
    Command cmd1;
    cmd1.name = "cd";
    REQUIRE(isBuiltin(cmd1) == true);

    Command cmd2;
    cmd2.name = "etc";
    REQUIRE(isBuiltin(cmd2) == true);

    Command cmd3;
    cmd3.name = "ls";
    REQUIRE(isBuiltin(cmd3) == false);
}