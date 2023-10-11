//
// (c) Bit Parallel Ltd, October 2023
//

#include <utility>

//#include "stm32.hpp"
#include "stm32_f446xx_irq_n.hpp"
#include "stm32_nvic.hpp"

#include "pendsv_handler.hpp"

PendSV& PendSV::getInstance(Listener&& irqListener, const uint8_t priority)
{
    static PendSV instance = PendSV(std::move(irqListener), priority);
    return instance;
}

PendSV::PendSV(Listener&& irqListener, const uint8_t priority)
{
    listener = std::ref(irqListener);

//  Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 2);   // use system clock
//  Stm32::sysTick(Stk::CTRL) = Stm32::sysTick(Stk::CTRL) | (1 << 1);   // enable the IRQ
//  Stm32::sysTick(Stk::LOAD) = 180 * timeBase;                         // timeBase is specified in us
//  Stm32::sysTick(Stk::VAL) = 0;

    // set the IRQ priority, hi --> low encoded as 0 --> 15 and enable the IRQ
    //
    Nvic::setPriority(Stm32F446IRQn::PendSV, priority);
}

void PendSV::handler()
{
    listener.get().irq();
}

void PendSV::enable()
{
}

void PendSV::disable()
{
}
