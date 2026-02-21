#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "executor.h"
#include "parser.h"
#include <fstream>

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

TEST_CASE("Executor handles output redirection", "[executor][redirection]") {
    Executor executor;
    Parser parser;
    Command cmd = parser.parse("echo hello");
    cmd.output_file = "test_output.txt";
    executor.execute(cmd);

    std::ifstream out("test_output.txt");
    std::string line;
    std::getline(out, line);
    out.close();
    REQUIRE(line == "hello");
    remove("test_output.txt");
}

TEST_CASE("Executor handles input redirection", "[executor][redirection]") {
    std::ofstream in("test_input.txt");
    in << "world";
    in.close();

    Executor executor;
    Parser parser;
    Command cmd = parser.parse("cat");
    cmd.input_file = "test_input.txt";
    cmd.output_file = "test_output.txt";
    executor.execute(cmd);

    std::ifstream out("test_output.txt");
    std::string line;
    std::getline(out, line);
    out.close();
    REQUIRE(line == "world");
    remove("test_input.txt");
    remove("test_output.txt");
}
