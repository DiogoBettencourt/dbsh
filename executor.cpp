#include "executor.h"
#include "builtins.h"
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <fcntl.h>

int Executor::execute(const Command& cmd) {
    if (isBuiltin(cmd)) {
        runBuiltin(cmd);
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        std::vector<char*> args;
        args.push_back(const_cast<char*>(cmd.name.c_str()));
        for (auto &arg : cmd.args) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);

        if (!cmd.input_file.empty()) {
            int fd = open(cmd.input_file.c_str(), O_RDONLY);
            if (fd < 0) {
                perror("open input file failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (!cmd.output_file.empty()) {
            int fd = open(cmd.output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open output file failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cmd.name.c_str(), args.data());

        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        if (!cmd.background) {
            waitpid(pid, nullptr, 0);
        }
    }
    return 0;
}