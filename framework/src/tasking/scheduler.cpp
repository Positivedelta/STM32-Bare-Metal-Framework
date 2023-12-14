//
// (c) Bit Parallel Ltd, December 2023
//

#include <utility>

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/f446xx_irq_n.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/stk.hpp"

#include "framework/tasking/scheduler.hpp"

bpl::TaskScheduler::TaskScheduler():
    reloadValue(0x00ffffff), maxUsedCycles(0) {
}

bpl::TaskScheduler& bpl::TaskScheduler::getInstance()
{
    static auto instance = bpl::TaskScheduler();
    return instance;
}

bpl::TaskScheduler& bpl::TaskScheduler::initialise(const uint32_t timeBase, TaskList&& taskList, const uint8_t priority)
{
    tasks = std::move(taskList);
    Nvic::setPriority(F446IRQn::SysTick, priority);

    reloadValue = (180 * timeBase) - 1;
    Stm32f4::sysTick(Stk::LOAD) = reloadValue;                              // the timeBase is specified in us
    Stm32f4::sysTick(Stk::VAL) = 0;
    Stm32f4::sysTick(Stk::CTRL) = Stm32f4::sysTick(Stk::CTRL) | (1 << 2);   // use the system clock
    Stm32f4::sysTick(Stk::CTRL) = Stm32f4::sysTick(Stk::CTRL) | (1 << 1);   // enable the IRQ

    return *this;
}

void bpl::TaskScheduler::start() const
{
    Stm32f4::sysTick(Stk::CTRL) = Stm32f4::sysTick(Stk::CTRL) | (1 << 0);   // enable SysTick
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

__attribute__((flatten, hot)) void SysTickIRQ::handler()
{
    auto& instance = bpl::TaskScheduler::getInstance();

    volatile uint32_t lastSysTick = instance.reloadValue;
    for (auto& schedulerTask : instance)
    {
        auto& task = schedulerTask.get();
        task.run();

        volatile uint32_t now = Stm32f4::sysTick(Stk::VAL);
        task.setUsedCycles(lastSysTick - now);
        lastSysTick = now;
    }

    volatile auto usedCycles = instance.reloadValue - Stm32f4::sysTick(Stk::VAL);
    if (usedCycles > instance.maxUsedCycles) instance.maxUsedCycles = usedCycles;
}
