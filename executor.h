#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

class Executor {
    public:
        int execute(const Command& cmd);
        int executePipeline(const std::vector<Command>& pipeline);
};

#endif