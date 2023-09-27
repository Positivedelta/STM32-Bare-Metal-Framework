//
// (c) Bit Parallel Ltd, 2021
//

#include "stm32.hpp"
#include "stm32_systick.hpp"
#include "systick_delay_handler.hpp"

SysTick::SysTick(const uint32_t delayMillis) : delayMillis(delayMillis)
{
    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 2);       // use system clock
    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 1);       // enable the IRQ
    Stm32::sysTick(Stk::LOAD) = 180000000 / 1000;                           // to generate a 1ms IRQ
    Stm32::sysTick(Stk::VAL) = 0;
    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 0);       // enable SysTick
}

void SysTick::handler()
{
    if (ticks != 0)
    {
        ticks = ticks - 1;
    }
}

void SysTick::delay()
{
    ticks = delayMillis;
    while (ticks != 0);
}
