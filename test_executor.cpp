#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "executor.h"
#include "parser.h"
#include <fstream>
#include <unistd.h>
#include <sstream>

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
    Command cmd = parser.parse("echo hello > test_output.txt");
    executor.execute(cmd);

    std::ifstream out("test_output.txt");
    std::string line;
    std::getline(out, line);
    out.close();
    REQUIRE(line == "hello");
    unlink("test_output.txt");
}

TEST_CASE("Executor handles input redirection", "[executor][redirection]") {
    std::ofstream in("test_input.txt");
    in << "world";
    in.close();

    Executor executor;
    Parser parser;
    Command cmd = parser.parse("cat < test_input.txt > test_output.txt");
    executor.execute(cmd);

    std::ifstream out("test_output.txt");
    std::string line;
    std::getline(out, line);
    out.close();
    REQUIRE(line == "world");
    unlink("test_input.txt");
    unlink("test_output.txt");
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
    Parser parser;
    
    // Create test file with content
    std::ofstream testFile("test_pipeline_input.txt");
    testFile << "hello\nworld\ntest\n";
    testFile.close();
    
    auto pipeline = parser.parsePipeline("cat test_pipeline_input.txt | wc -l > test_pipeline.txt");
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_pipeline.txt");
    std::string content;
    std::getline(file, content);
    file.close();
    
    REQUIRE(content.find("3") != std::string::npos);
    
    unlink("test_pipeline_input.txt");
    unlink("test_pipeline.txt");
}

TEST_CASE("Executor::executePipeline - three command pipeline", "[executor][pipeline]") {
    Executor executor;
    Parser parser;
    
    // Create test file
    std::ofstream testFile("test_triple_input.txt");
    testFile << "apple\nbanana\napricot\ncherry\n";
    testFile.close();
    
    auto pipeline = parser.parsePipeline("cat test_triple_input.txt | grep ^a | wc -l > test_triple.txt");
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_triple.txt");
    std::string content;
    std::getline(file, content);
    file.close();
    
    REQUIRE(content.find("2") != std::string::npos);
    
    unlink("test_triple_input.txt");
    unlink("test_triple.txt");
}

TEST_CASE("Executor::executePipeline - with input redirection", "[executor][pipeline]") {
    Executor executor;
    Parser parser;
    
    // Create test input file
    std::ofstream input_file("test_input.txt");
    input_file << "line1\nline2\nline3\n";
    input_file.close();
    
    auto pipeline = parser.parsePipeline("cat < test_input.txt | wc -l > test_input_redir.txt");
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_input_redir.txt");
    std::string content;
    std::getline(file, content);
    file.close();
    
    REQUIRE(content.find("3") != std::string::npos);
    
    unlink("test_input.txt");
    unlink("test_input_redir.txt");
}

TEST_CASE("Executor::executePipeline - complex pipeline (cat | grep | sort)", "[executor][pipeline]") {
    Executor executor;
    Parser parser;
    
    // Create test file
    std::ofstream input_file("test_complex.txt");
    input_file << "zebra\napple\nbanana\napricot\nzoo\n";
    input_file.close();
    
    auto pipeline = parser.parsePipeline("cat test_complex.txt | grep ^a | sort > test_complex_out.txt");
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_complex_out.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();
    
    REQUIRE(content == "apple\napricot\n");
    
    unlink("test_complex.txt");
    unlink("test_complex_out.txt");
}

TEST_CASE("Executor::executePipeline - echo with newlines", "[executor][pipeline]") {
    Executor executor;
    Parser parser;
    
    // Use printf for more reliable newline handling
    auto pipeline = parser.parsePipeline("printf 'line1\\nline2\\nline3\\n' | wc -l > test_newlines.txt");
    REQUIRE(executor.executePipeline(pipeline) == 0);
    
    std::ifstream file("test_newlines.txt");
    std::string content;
    std::getline(file, content);
    file.close();
    
    REQUIRE(content.find("3") != std::string::npos);
    
    unlink("test_newlines.txt");
}

TEST_CASE("Executor handles background process", "[executor][background]") {
    Executor executor;
    Command cmd;
    cmd.name = "sleep";
    cmd.args = {"1"};
    cmd.background = true;
    
    int result = executor.execute(cmd);
    REQUIRE(result == 0);
    // Process should run in background, not blocking
}
