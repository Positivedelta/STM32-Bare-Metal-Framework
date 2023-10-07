//
// (c) Bit Parallel Ltd, September 2023
//

#include <utility>

#include "stm32.hpp"
#include "stm32_systick.hpp"

#include "systick_handler.hpp"

SysTick& SysTick::getInstance(const uint32_t timeBase, bpl::IrqListeners&& irqListeners)
{
    static SysTick instance = SysTick(timeBase, std::move(irqListeners));
    return instance;
}

SysTick::SysTick(const uint32_t timeBase, bpl::IrqListeners&& irqListeners)
{
    timeIncrement = timeBase;
    listeners = std::move(irqListeners);

    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 2);   // use system clock
    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 1);   // enable the IRQ
    Stm32::sysTick(Stk::LOAD) = 180 * timeBase;                         // timeBase is specified in us
    Stm32::sysTick(Stk::VAL) = 0;
}

void SysTick::handler()
{
    for (const auto& listener : listeners) listener.get().tryFire(timeIncrement);
}

void SysTick::enable()
{
    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 0);       // enable SysTick
}

void SysTick::disable()
{
    //
    // FIXME! implement...
    //
}
