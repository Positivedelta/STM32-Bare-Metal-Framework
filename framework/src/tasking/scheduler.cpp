//
// (c) Bit Parallel Ltd, December 2023
//

#include <cstdio>
#include <utility>

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/f446xx_irq_n.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/stk.hpp"
#include "framework/io/ascii.hpp"
#include "framework/utils/string_utils.hpp"

#include "framework/tasking/scheduler.hpp"

bpl::TaskScheduler::TaskScheduler():
    CliProvider("tasks", "[#refresh]"),
    schedulerTimeBase(10'000), reloadValue(0x00ffffff), maxUsedCycles(0) {
}

bpl::TaskScheduler& bpl::TaskScheduler::getInstance()
{
    static auto instance = bpl::TaskScheduler();
    return instance;
}

bpl::TaskScheduler& bpl::TaskScheduler::initialise(const uint32_t timeBase, TaskList&& taskList, const uint8_t priority)
{
    schedulerTimeBase = timeBase;
    tasks = std::move(taskList);
    Nvic::setPriority(F446IRQn::SysTick, priority);

    reloadValue = (180 * timeBase) - 1;
    Stm32f4::sysTick(Stk::LOAD) = reloadValue;                                          // the timeBase is specified in us
    Stm32f4::sysTick(Stk::VAL) = 0;
    Stm32f4::sysTick(Stk::CTRL) = Stm32f4::sysTick(Stk::CTRL) | Stk::CTRL_CLKSOURCE;    // use the system clock
    Stm32f4::sysTick(Stk::CTRL) = Stm32f4::sysTick(Stk::CTRL) | Stk::CTRL_TICKINT;      // enable the IRQ

    return *this;
}

void bpl::TaskScheduler::start() const
{
    Stm32f4::sysTick(Stk::CTRL) = Stm32f4::sysTick(Stk::CTRL) | Stk::CTRL_ENABLE;       // enable SysTick
    Nvic::enableIRQ(F446IRQn::SysTick);
}

uint32_t bpl::TaskScheduler::getAvailableCycles() const
{
    return reloadValue;
}

uint32_t bpl::TaskScheduler::getMaxUsedCycles() const
{
    return maxUsedCycles;
}

float bpl::TaskScheduler::getMaxCpu() const
{
    return (100 * maxUsedCycles) / float(reloadValue);
}

//
// the SysTick IRQ handler
//

__attribute__((flatten, hot)) void SysTick::handler()
{
    auto& instance = bpl::TaskScheduler::getInstance();

    volatile uint32_t lastSysTick = instance.reloadValue;
    for (auto& schedulerTask : instance)
    {
        auto& task = schedulerTask.get();
        task.invoke();

        volatile uint32_t now = Stm32f4::sysTick(Stk::VAL);
        task.setUsedCycles(lastSysTick - now);
        lastSysTick = now;
    }

    volatile auto usedCycles = instance.reloadValue - Stm32f4::sysTick(Stk::VAL);
    if (usedCycles > instance.maxUsedCycles) instance.maxUsedCycles = usedCycles;
}

//
// the inherited CLI handler
//

bool bpl::TaskScheduler::handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console)
{
    const auto& consoleWriter = console.getPrintWriter();
    const auto& consoleReader = console.getTextReader();

    // allowed syntax: tasks [#refresh]
    //
    if (commandTokens.size() == 1)
    {
        printTaskStatistics(consoleWriter);
        return true;
    }

    if (commandTokens.size() == 2)
    {
        int32_t refresh = 0;
        if (bpl::StringUtils::stoi(commandTokens[1], refresh))
        {
            // check every 1/4 second for an ESC key press, i.e. to quit
            //
            auto running = true;
            while (running)
            {
                printTaskStatistics(consoleWriter);
                consoleWriter.println();

                for (auto interval = 0; interval < refresh * 4; interval++)
                {
                    if (consoleReader.isKey(bpl::Ascii::ESC))
                    {
                        running = false;
                        break;
                    }

                    // wait for 1/4 second
                    // notes 1, SysTick is configured to count in microseconds
                    //       2, the timebase value represents the SysTick countdown period and is specified in microseconds
                    //       3, it takes 1s to for SysTick to cycle "timebase" times 
                    //
                    uint32_t sysTickCounts = 0;
                    while (sysTickCounts < (schedulerTimeBase >> 2))
                    {
                        while ((Stm32f4::sysTick(Stk::CTRL) & Stk::CTRL_COUNT) > 0) sysTickCounts++;
                    }
                }
            }

            return true;
        }
    }

    return false;
}

void bpl::TaskScheduler::printTaskStatistics(const bpl::PrintWriter& consoleWriter)
{
    std::snprintf(taskStatistics, sizeof(taskStatistics), "Scheduler CPU: %.2f%% [%lu Cycles]", getMaxCpu(), getMaxUsedCycles());
    consoleWriter.println(taskStatistics);

    consoleWriter.println("Task CPU:");
    const auto availableCycles = getAvailableCycles();
    for (auto& schedulerTask : *this)
    {
        auto& task = schedulerTask.get();
        std::snprintf(taskStatistics, sizeof(taskStatistics), "    %5.2f%% [%s]", task.getMaxCpu(availableCycles), task.getTaskName());
        consoleWriter.println(taskStatistics);
    }
}
