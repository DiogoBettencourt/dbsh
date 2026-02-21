#include "executor.h"
#include "builtins.h"
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

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