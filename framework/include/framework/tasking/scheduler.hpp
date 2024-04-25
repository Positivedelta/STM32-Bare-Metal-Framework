//
// (c) Bit Parallel Ltd, November 2023
//

#ifndef BPL_FRAMEWORK_TASK_SCHEDULER_H
#define BPL_FRAMEWORK_TASK_SCHEDULER_H

#include <cstdint>
#include <functional>
#include <memory_resource>
#include <vector>

#include "framework/cli/cli_provider.hpp"
#include "framework/io/print_writer.hpp"
#include "framework/tasking/task.hpp"

class SysTick
{
    public:
        static void handler();
};

namespace bpl
{
    class TaskScheduler : public bpl::CliProvider
    {
        friend void ::SysTick::handler();

        using WrappedTask = std::reference_wrapper<bpl::Task>;
        using Tasks = std::pmr::vector<WrappedTask>;

        private:
            class TaskList : public Tasks
            {
                public:
                    template <typename ...T>
                    TaskList(T&& ...t):
                        std::pmr::vector<WrappedTask>{std::forward<T>(t)...} {
                    }
            };

            TaskList tasks;
            uint32_t schedulerTimeBase, reloadValue;
            volatile uint32_t maxUsedCycles;
            char taskStatistics[64];

        public:
            static TaskScheduler& getInstance();

            TaskScheduler& initialise(const uint32_t timeBase, TaskList&& taskList, const uint8_t priority);
            void start() const;
            uint32_t getAvailableCycles() const;
            uint32_t getMaxUsedCycles() const;
            float getMaxCpu() const;

            //
            // deleted constructors, as this class is a singleton
            //

            TaskScheduler(const TaskScheduler&) = delete;
            TaskScheduler(const TaskScheduler()&) = delete;
            TaskScheduler& operator=(const TaskScheduler&) = delete;
            void operator=(const TaskScheduler()&) = delete;

            TaskScheduler(TaskScheduler&) = delete;
            TaskScheduler(TaskScheduler()&) = delete;
            TaskScheduler& operator=(TaskScheduler&) = delete;
            void operator=(TaskScheduler()&) = delete;

            //
            // iterators to allow the use of for in loops
            //

            Tasks::iterator begin() noexcept
            {
                return tasks.begin();
            }

            Tasks::iterator end() noexcept
            {
                return tasks.end();
            }

            Tasks::const_iterator cbegin() const noexcept
            {
                return tasks.cbegin();
            }

            Tasks::const_iterator cend() const noexcept
            {
                return tasks.cend();
            }

        private:
            TaskScheduler();
            bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console) override;
            void printTaskStatistics(const bpl::PrintWriter& consoleWriter);
    };
}

#endif
