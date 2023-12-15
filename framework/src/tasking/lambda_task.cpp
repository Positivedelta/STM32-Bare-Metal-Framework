//
// (c) Bit Parallel Ltd, December 2023
//

#include "framework/tasking/lambda_task.hpp"

bpl::LambdaTask::LambdaTask(const uint32_t period, const char* taskName, std::function<void()> task):
    Task(period, taskName), task(task) {
}

void bpl::LambdaTask::runTask()
{
    task();
}
