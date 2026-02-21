#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "builtins.h"
#include <unistd.h>
#include <climits>
#include <cstdlib>

TEST_CASE("isBuiltin identifies built-in commands", "[builtins]") {
    Command cmd1;
    cmd1.name = "cd";
    REQUIRE(isBuiltin(cmd1) == true);

    Command cmd2;
    cmd2.name = "exit";
    REQUIRE(isBuiltin(cmd2) == true);

    Command cmd3;
    cmd3.name = "pwd";
    REQUIRE(isBuiltin(cmd3) == true);

    Command cmd4;
    cmd4.name = "echo";
    REQUIRE(isBuiltin(cmd4) == true);

    Command cmd5;
    cmd5.name = "export";
    REQUIRE(isBuiltin(cmd5) == true);

    Command cmd6;
    cmd6.name = "unset";
    REQUIRE(isBuiltin(cmd6) == true);

    Command cmd7;
    cmd7.name = "ls";
    REQUIRE(isBuiltin(cmd7) == false);

    Command cmd8;
    cmd8.name = "grep";
    REQUIRE(isBuiltin(cmd8) == false);
}

TEST_CASE("cd changes directory", "[builtins][cd]") {
    char original_dir[PATH_MAX];
    getcwd(original_dir, sizeof(original_dir));

    SECTION("cd to home directory") {
        Command cmd;
        cmd.name = "cd";
        // No arguments - should go to HOME
        
        dbsh_cd(cmd);
        
        char new_dir[PATH_MAX];
        getcwd(new_dir, sizeof(new_dir));
        
        const char* home = getenv("HOME");
        REQUIRE(std::string(new_dir) == std::string(home));
    }

    SECTION("cd to /tmp") {
        Command cmd;
        cmd.name = "cd";
        cmd.args.push_back("/tmp");
        
        dbsh_cd(cmd);
        
        char new_dir[PATH_MAX];
        getcwd(new_dir, sizeof(new_dir));
        
        REQUIRE(std::string(new_dir) == "/tmp");
    }

    SECTION("cd updates PWD environment variable") {
        Command cmd;
        cmd.name = "cd";
        cmd.args.push_back("/tmp");
        
        dbsh_cd(cmd);
        
        const char* pwd = getenv("PWD");
        REQUIRE(pwd != nullptr);
        REQUIRE(std::string(pwd) == "/tmp");
    }

    // Restore original directory
    chdir(original_dir);
}

TEST_CASE("pwd prints current directory", "[builtins][pwd]") {
    Command cmd;
    cmd.name = "pwd";
    
    // This test just ensures pwd doesn't crash
    // Actual output would need to be captured to test fully
    REQUIRE_NOTHROW(dbsh_pwd(cmd));
}

TEST_CASE("echo prints arguments", "[builtins][echo]") {
    Command cmd;
    cmd.name = "echo";
    
    SECTION("echo with no arguments") {
        // Should just print newline, no crash
        REQUIRE_NOTHROW(dbsh_echo(cmd));
    }

    SECTION("echo with single argument") {
        cmd.args.push_back("hello");
        REQUIRE_NOTHROW(dbsh_echo(cmd));
    }

    SECTION("echo with multiple arguments") {
        cmd.args.push_back("hello");
        cmd.args.push_back("world");
        cmd.args.push_back("test");
        REQUIRE_NOTHROW(dbsh_echo(cmd));
    }
}

TEST_CASE("export sets environment variables", "[builtins][export]") {
    SECTION("export sets single variable") {
        Command cmd;
        cmd.name = "export";
        cmd.args.push_back("TEST_VAR=test_value");
        
        dbsh_export(cmd);
        
        const char* value = getenv("TEST_VAR");
        REQUIRE(value != nullptr);
        REQUIRE(std::string(value) == "test_value");
        
        unsetenv("TEST_VAR");
    }

    SECTION("export sets multiple variables") {
        Command cmd;
        cmd.name = "export";
        cmd.args.push_back("VAR1=value1");
        cmd.args.push_back("VAR2=value2");
        
        dbsh_export(cmd);
        
        const char* val1 = getenv("VAR1");
        const char* val2 = getenv("VAR2");
        REQUIRE(val1 != nullptr);
        REQUIRE(val2 != nullptr);
        REQUIRE(std::string(val1) == "value1");
        REQUIRE(std::string(val2) == "value2");
        
        unsetenv("VAR1");
        unsetenv("VAR2");
    }

    SECTION("export with empty value") {
        Command cmd;
        cmd.name = "export";
        cmd.args.push_back("EMPTY_VAR=");
        
        dbsh_export(cmd);
        
        const char* value = getenv("EMPTY_VAR");
        REQUIRE(value != nullptr);
        REQUIRE(std::string(value) == "");
        
        unsetenv("EMPTY_VAR");
    }

    SECTION("export with no arguments prints error") {
        Command cmd;
        cmd.name = "export";
        // No args - should print error but not crash
        REQUIRE_NOTHROW(dbsh_export(cmd));
    }

    SECTION("export with invalid format prints error") {
        Command cmd;
        cmd.name = "export";
        cmd.args.push_back("INVALID");
        // Invalid format - should print error but not crash
        REQUIRE_NOTHROW(dbsh_export(cmd));
    }
}

TEST_CASE("unset removes environment variables", "[builtins][unset]") {
    SECTION("unset removes existing variable") {
        setenv("TEST_UNSET", "value", 1);
        
        Command cmd;
        cmd.name = "unset";
        cmd.args.push_back("TEST_UNSET");
        
        dbsh_unset(cmd);
        
        const char* value = getenv("TEST_UNSET");
        REQUIRE(value == nullptr);
    }

    SECTION("unset removes multiple variables") {
        setenv("UNSET1", "val1", 1);
        setenv("UNSET2", "val2", 1);
        
        Command cmd;
        cmd.name = "unset";
        cmd.args.push_back("UNSET1");
        cmd.args.push_back("UNSET2");
        
        dbsh_unset(cmd);
        
        REQUIRE(getenv("UNSET1") == nullptr);
        REQUIRE(getenv("UNSET2") == nullptr);
    }

    SECTION("unset with no arguments prints error") {
        Command cmd;
        cmd.name = "unset";
        // No args - should print error but not crash
        REQUIRE_NOTHROW(dbsh_unset(cmd));
    }

    SECTION("unset non-existent variable doesn't crash") {
        Command cmd;
        cmd.name = "unset";
        cmd.args.push_back("NON_EXISTENT_VAR_12345");
        
        REQUIRE_NOTHROW(dbsh_unset(cmd));
    }
}

TEST_CASE("runBuiltin dispatches to correct function", "[builtins]") {
    SECTION("runBuiltin handles cd") {
        Command cmd;
        cmd.name = "cd";
        REQUIRE_NOTHROW(runBuiltin(cmd));
    }

    SECTION("runBuiltin handles pwd") {
        Command cmd;
        cmd.name = "pwd";
        REQUIRE_NOTHROW(runBuiltin(cmd));
    }

    SECTION("runBuiltin handles echo") {
        Command cmd;
        cmd.name = "echo";
        cmd.args.push_back("test");
        REQUIRE_NOTHROW(runBuiltin(cmd));
    }

    SECTION("runBuiltin handles export") {
        Command cmd;
        cmd.name = "export";
        cmd.args.push_back("TEST=value");
        REQUIRE_NOTHROW(runBuiltin(cmd));
        unsetenv("TEST");
    }

    SECTION("runBuiltin handles unset") {
        setenv("TEMP_VAR", "value", 1);
        Command cmd;
        cmd.name = "unset";
        cmd.args.push_back("TEMP_VAR");
        REQUIRE_NOTHROW(runBuiltin(cmd));
    }
}