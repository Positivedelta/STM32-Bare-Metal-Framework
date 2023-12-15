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
        private:
            const uint32_t period;
            uint32_t counter;
            uint32_t maxUsedCycles;
            char name[32];

        protected:
            Task(const uint32_t period, const char* taskName);
            virtual void runTask() = 0;

        public:
            void invoke();
            void setUsedCycles(uint32_t usedCycles);
            float getMaxCpu(const uint32_t availableCycles) const;
            const char* getTaskName() const;
    };
}

#endif
