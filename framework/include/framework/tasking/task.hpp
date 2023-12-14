//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_FRAMEWORK_TASK_H
#define BPL_FRAMEWORK_TASK_H

#include <cstdint>

namespace bpl
{
    class Task
    {
        public:
            virtual void run() = 0;
            virtual void setUsedCycles(uint32_t usedCycles) = 0;
            virtual float getMaxCpu(const uint32_t availableCycles) const = 0;
            virtual const char* getName() const = 0;
    };
}

#endif
