//
// (c) Bit Parallel Ltd, December 2023
//

#include <cstdio>

#include "framework/tasking/task.hpp"

// note, std::snprintf() will truncate the name if it's longer than the class property
//
bpl::Task::Task(const uint32_t period, const char* taskName):
    period(period), counter(period), maxUsedCycles(0) {
        std::snprintf(name, sizeof(name), taskName);
}

void bpl::Task::invoke()
{
    if (--counter > 0) return;

    runTask();
    counter = period;
}

void bpl::Task::setUsedCycles(uint32_t usedCycles)
{
    if (usedCycles > maxUsedCycles) maxUsedCycles = usedCycles;
}

float bpl::Task::getMaxCpu(const uint32_t availableCycles) const
{
    return (100 * maxUsedCycles) / float(availableCycles);
}

const char* bpl::Task::getTaskName() const
{
    return name;
}
