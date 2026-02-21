#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "executor.h"
#include "parser.h"
#include <fstream>
#include <unistd.h>

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

TEST_CASE("Executor::executePipeline - empty pipeline", "[executor][pipeline]") {
    Executor executor;
    std::vector<Command> pipeline;
    
    REQUIRE(executor.executePipeline(pipeline) == 0);
}

TEST_CASE("Executor::executePipeline - single command", "[executor][pipeline]") {
    Executor executor;
    Command cmd;
    cmd.name = "echo";
    cmd.args = {"hello"};
    cmd.output_file = "test_single.txt";
    cmd.background = false;
    
    std::vector<Command> pipeline = {cmd};
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_single.txt");
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    REQUIRE(content == "hello\n");
    
    unlink("test_single.txt");
}

TEST_CASE("Executor::executePipeline - two command pipeline", "[executor][pipeline]") {
    Executor executor;
    
    Command cmd1;
    cmd1.name = "echo";
    cmd1.args = {"hello\nworld"};
    
    Command cmd2;
    cmd2.name = "wc";
    cmd2.args = {"-l"};
    cmd2.output_file = "test_pipeline.txt";
    
    std::vector<Command> pipeline = {cmd1, cmd2};
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_pipeline.txt");
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    REQUIRE(content.find("2") != std::string::npos);
    
    unlink("test_pipeline.txt");
}

TEST_CASE("Executor::executePipeline - three command pipeline", "[executor][pipeline]") {
    Executor executor;
    
    Command cmd1;
    cmd1.name = "echo";
    cmd1.args = {"apple\nbanana\napricot"};
    
    Command cmd2;
    cmd2.name = "grep";
    cmd2.args = {"^a"};
    
    Command cmd3;
    cmd3.name = "wc";
    cmd3.args = {"-l"};
    cmd3.output_file = "test_triple.txt";
    
    std::vector<Command> pipeline = {cmd1, cmd2, cmd3};
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_triple.txt");
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    REQUIRE(content.find("2") != std::string::npos);
    
    unlink("test_triple.txt");
}

TEST_CASE("Executor::executePipeline - with input redirection", "[executor][pipeline]") {
    Executor executor;
    
    // Create test input file
    std::ofstream input_file("test_input.txt");
    input_file << "line1\nline2\nline3\n";
    input_file.close();
    
    Command cmd1;
    cmd1.name = "cat";
    cmd1.input_file = "test_input.txt";
    
    Command cmd2;
    cmd2.name = "wc";
    cmd2.args = {"-l"};
    cmd2.output_file = "test_input_redir.txt";
    
    std::vector<Command> pipeline = {cmd1, cmd2};
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_input_redir.txt");
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    REQUIRE(content.find("3") != std::string::npos);
    
    unlink("test_input.txt");
    unlink("test_input_redir.txt");
}

TEST_CASE("Executor::executePipeline - complex pipeline (cat | grep | sort)", "[executor][pipeline]") {
    Executor executor;
    
    // Create test file
    std::ofstream input_file("test_complex.txt");
    input_file << "zebra\napple\nbanana\napricot\nzoo\n";
    input_file.close();
    
    Command cmd1;
    cmd1.name = "cat";
    cmd1.input_file = "test_complex.txt";
    
    Command cmd2;
    cmd2.name = "grep";
    cmd2.args = {"^a"};
    
    Command cmd3;
    cmd3.name = "sort";
    cmd3.output_file = "test_complex_out.txt";
    
    std::vector<Command> pipeline = {cmd1, cmd2, cmd3};
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_complex_out.txt");
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    REQUIRE(content == "apple\napricot\n");
    
    unlink("test_complex.txt");
    unlink("test_complex_out.txt");
}

TEST_CASE("Executor::executePipeline - command not found", "[executor][pipeline]") {
    Executor executor;
    
    Command cmd1;
    cmd1.name = "echo";
    cmd1.args = {"test"};
    
    Command cmd2;
    cmd2.name = "nonexistentcommand12345";
    
    std::vector<Command> pipeline = {cmd1, cmd2};
    
    // Should handle errors gracefully
    executor.executePipeline(pipeline);
    // Note: Actual behavior depends on error handling implementation
}

TEST_CASE("Executor::executePipeline - with builtin command", "[executor][pipeline]") {
    Executor executor;
    
    Command cmd1;
    cmd1.name = "echo";
    cmd1.args = {"test"};
    
    // Assuming 'cd' is a builtin
    Command cmd2;
    cmd2.name = "cd";
    cmd2.args = {"/"};
    
    std::vector<Command> pipeline = {cmd1, cmd2};
    
    // Should handle builtins in pipeline
    REQUIRE(executor.executePipeline(pipeline) == 0);
}
