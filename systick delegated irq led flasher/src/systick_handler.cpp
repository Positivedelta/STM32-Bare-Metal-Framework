//
// (c) Bit Parallel Ltd, September 2023
//

#include <utility>

#include "stm32.hpp"
#include "stm32_systick.hpp"

#include "systick_handler.hpp"

SysTick::SysTick(bpl::IrqListeners&& irqListeners)
{
    listeners = std::move(irqListeners);

    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 2);       // use system clock
    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 1);       // enable the IRQ
    Stm32::sysTick(Stk::LOAD) = 180000000 / 1000;                           // to generate a 1ms IRQ
    Stm32::sysTick(Stk::VAL) = 0;
}

void SysTick::handler()
{
    for (auto& listener : listeners) listener.get().irq();
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
