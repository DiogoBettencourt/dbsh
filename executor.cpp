#include "executor.h"
#include "builtins.h"
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <fcntl.h>

int Executor::execute(const Command& cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        // Set up redirections first, before running builtin or external command
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

        // Now check if it's a builtin and run it
        if (isBuiltin(cmd)) {
            runBuiltin(cmd);
            exit(0);
        }

        // Otherwise, set up args and exec
        std::vector<char*> args;
        args.push_back(const_cast<char*>(cmd.name.c_str()));
        for (auto &arg : cmd.args) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);

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

int Executor::executePipeline(const std::vector<Command>& pipeline) {
    if (pipeline.empty()) {
        return 0;
    }

    if (pipeline.size() == 1) {
        return execute(pipeline[0]);
    }

    int numPipes = pipeline.size() - 1;
    std::vector<int[2]> pipes(numPipes);

    for (int i = 0; i < numPipes; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe failed");
            return -1;
        }
    }

    std::vector<pid_t> pids;

    for (size_t i = 0; i < pipeline.size(); i++) {
        const Command& cmd = pipeline[i];

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return -1;
        }

        if (pid == 0) {
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            if (i < pipeline.size() - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            for (int j = 0; j < numPipes; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

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

            if (isBuiltin(cmd)) {
                runBuiltin(cmd);
                exit(0);
            }

            std::vector<char*> args;
            args.push_back(const_cast<char*>(cmd.name.c_str()));
            for (auto &arg : cmd.args) {
                args.push_back(const_cast<char*>(arg.c_str()));
            }
            args.push_back(nullptr);

            execvp(cmd.name.c_str(), args.data());
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }

        pids.push_back(pid);
    }

    for (int i = 0; i < numPipes; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    bool is_background = !pipeline.empty() && pipeline.back().background;
    
    if (!is_background) {
        for (pid_t pid : pids) {
            waitpid(pid, nullptr, 0);
        }
    }

    return 0;
}