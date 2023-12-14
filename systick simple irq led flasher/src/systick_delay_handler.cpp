//
// (c) Bit Parallel Ltd, 2021
//

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/stk.hpp"

#include "systick_delay_handler.hpp"

SysTick::SysTick(const uint32_t delayMillis) : delayMillis(delayMillis)
{
    Stm32f4::sysTick(Stk::CTRL) = Stm32f4::sysTick(Stk::CTRL) | (1 << 2);       // use system clock
    Stm32f4::sysTick(Stk::CTRL) = Stm32f4::sysTick(Stk::CTRL) | (1 << 1);       // enable the IRQ
    Stm32f4::sysTick(Stk::LOAD) = 180000000 / 1000;                             // to generate a 1ms IRQ
    Stm32f4::sysTick(Stk::VAL) = 0;
    Stm32f4::sysTick(Stk::CTRL) = Stm32f4::sysTick(Stk::CTRL) | (1 << 0);       // enable SysTick
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
