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

TEST_CASE("Parser handles input with only spaces", "[parser]") {
    Parser parser;
    auto cmd = parser.parse("   ");
    REQUIRE(cmd.name == "");
    REQUIRE(cmd.args.empty());
}

TEST_CASE("Parser handles input redirection", "[parser]") {
    Parser parser;
    auto cmd = parser.parse("cat < input.txt");
    REQUIRE(cmd.name == "cat");
    REQUIRE(cmd.input_file == "input.txt");
    REQUIRE(cmd.args.empty());
}

TEST_CASE("Parser handles output redirection", "[parser]") {
    Parser parser;
    auto cmd = parser.parse("echo hello > output.txt");
    REQUIRE(cmd.name == "echo");
    REQUIRE(cmd.output_file == "output.txt");
    REQUIRE(cmd.args.size() == 1);
    REQUIRE(cmd.args[0] == "hello");
}

TEST_CASE("Parser handles background process", "[parser]") {
    Parser parser;
    auto cmd = parser.parse("sleep 10 &");
    REQUIRE(cmd.name == "sleep");
    REQUIRE(cmd.args.size() == 1);
    REQUIRE(cmd.args[0] == "10");
    REQUIRE(cmd.background == true);
}

TEST_CASE("Parser handles input and output redirection", "[parser]") {
    Parser parser;
    auto cmd = parser.parse("sort < input.txt > output.txt");
    REQUIRE(cmd.name == "sort");
    REQUIRE(cmd.input_file == "input.txt");
    REQUIRE(cmd.output_file == "output.txt");
}

TEST_CASE("Parser::parsePipeline - single command", "[parser][pipeline]") {
    Parser parser;
    auto pipeline = parser.parsePipeline("ls -la");
    REQUIRE(pipeline.size() == 1);
    REQUIRE(pipeline[0].name == "ls");
    REQUIRE(pipeline[0].args.size() == 1);
    REQUIRE(pipeline[0].args[0] == "-la");
}

TEST_CASE("Parser::parsePipeline - two commands", "[parser][pipeline]") {
    Parser parser;
    auto pipeline = parser.parsePipeline("cat file.txt | grep error");
    REQUIRE(pipeline.size() == 2);
    REQUIRE(pipeline[0].name == "cat");
    REQUIRE(pipeline[0].args.size() == 1);
    REQUIRE(pipeline[0].args[0] == "file.txt");
    REQUIRE(pipeline[1].name == "grep");
    REQUIRE(pipeline[1].args.size() == 1);
    REQUIRE(pipeline[1].args[0] == "error");
}

TEST_CASE("Parser::parsePipeline - three commands", "[parser][pipeline]") {
    Parser parser;
    auto pipeline = parser.parsePipeline("ls -l | grep txt | wc -l");
    REQUIRE(pipeline.size() == 3);
    REQUIRE(pipeline[0].name == "ls");
    REQUIRE(pipeline[0].args[0] == "-l");
    REQUIRE(pipeline[1].name == "grep");
    REQUIRE(pipeline[1].args[0] == "txt");
    REQUIRE(pipeline[2].name == "wc");
    REQUIRE(pipeline[2].args[0] == "-l");
}

TEST_CASE("Parser::parsePipeline - with redirections", "[parser][pipeline]") {
    Parser parser;
    auto pipeline = parser.parsePipeline("cat < input.txt | grep error | sort > output.txt");
    REQUIRE(pipeline.size() == 3);
    REQUIRE(pipeline[0].name == "cat");
    REQUIRE(pipeline[0].input_file == "input.txt");
    REQUIRE(pipeline[1].name == "grep");
    REQUIRE(pipeline[1].args[0] == "error");
    REQUIRE(pipeline[2].name == "sort");
    REQUIRE(pipeline[2].output_file == "output.txt");
}

TEST_CASE("Parser::parsePipeline - empty input", "[parser][pipeline]") {
    Parser parser;
    auto pipeline = parser.parsePipeline("");
    REQUIRE(pipeline.empty());
}

TEST_CASE("Parser::parsePipeline - with spaces around pipe", "[parser][pipeline]") {
    Parser parser;
    auto pipeline = parser.parsePipeline("ls  |  grep test  |  wc");
    REQUIRE(pipeline.size() == 3);
    REQUIRE(pipeline[0].name == "ls");
    REQUIRE(pipeline[1].name == "grep");
    REQUIRE(pipeline[1].args[0] == "test");
    REQUIRE(pipeline[2].name == "wc");
}
