//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_FRAMEWORK_LAMBDA_TASK_H
#define BPL_FRAMEWORK_LAMBDA_TASK_H

#include <cstdint>
#include <functional>

#include "framework/tasking/task.hpp"

namespace bpl
{
    class LambdaTask : public bpl::Task
    {
        private:
            std::function<void()> task;

        public:
            LambdaTask(const uint32_t period, const char* taskName, std::function<void()> task);
            void runTask() override;
    };
}

#endif
