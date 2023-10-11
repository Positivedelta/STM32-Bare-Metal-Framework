//
// (c) Bit Parallel Ltd, October 2023
//

#include <utility>

#include "stm32.hpp"
#include "stm32_f446xx_irq_n.hpp"
#include "stm32_nvic.hpp"
#include "stm32_systick.hpp"

#include "systick_handler.hpp"

SysTick& SysTick::getInstance(const int32_t timeBase, Listeners&& irqListeners, const uint8_t priority)
{
    static SysTick instance = SysTick(timeBase, std::move(irqListeners), priority);
    return instance;
}

SysTick::SysTick(const int32_t timeBase, Listeners&& irqListeners, const uint8_t priority)
{
    timeIncrement = timeBase;
    listeners = std::move(irqListeners);

    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 2);   // use system clock
    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 1);   // enable the IRQ
    Stm32::sysTick(Stk::LOAD) = 180 * timeBase;                         // timeBase is specified in us
    Stm32::sysTick(Stk::VAL) = 0;

    // set the IRQ priority, hi --> low encoded as 0 --> 15 and enable the IRQ
    //
    Nvic::setPriority(Stm32F446IRQn::SysTick, priority);
}

void SysTick::handler()
{
    for (const auto& listener : listeners) listener.get().tryFire(timeIncrement);
}

void SysTick::enable()
{
    Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 0);   // enable SysTick
}

void SysTick::disable()
{
    //
    // FIXME! implement...
    //
}
