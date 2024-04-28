//
// (c) Bit Parallel Ltd, December 2023
//

#include "framework/utils/string_utils.hpp"
#include "framework/tasking/task.hpp"

// note, bpl::StringUtils::ccpy() will truncate the name if it's longer than the class property
//
bpl::Task::Task(const uint32_t period, const char* taskName):
    period(period), counter(period), maxUsedCycles(0) {
        bpl::StringUtils::ccpy<sizeof(name)>(taskName, name);
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
