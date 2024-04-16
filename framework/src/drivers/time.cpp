//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/timer.hpp"
#include "framework/stm32f4/rcc.hpp"

#include "framework/drivers/time.hpp"

driver::Time& driver::Time::getInstance(const uint32_t clockSpeedMhz, const uint32_t priority)
{
    static auto instance = driver::Time(clockSpeedMhz, priority);
    return instance;
}

driver::Time::Time(const uint32_t clockSpeedMhz, const uint32_t priority)
{
    Stm32f4::rcc(Rcc::APB1ENR) = Stm32f4::rcc(Rcc::APB1ENR) | Rcc::APB1ENR_TIMER2EN;

    // timer off and reset
    //
    Stm32f4::timer2(Timer::CR1) = Stm32f4::timer2(Timer::CR1) & ~Timer::CR1_CEN;
    Stm32f4::rcc(Rcc::APB1RSTR) = Stm32f4::rcc(Rcc::APB1RSTR) | Rcc::APB1RSTR_TIMER2RST;
    Stm32f4::rcc(Rcc::APB1RSTR) = Stm32f4::rcc(Rcc::APB1RSTR) & ~Rcc::APB1RSTR_TIMER2RST;

    // configure to count in microseconds and generate an IRQ on overflow / reload
    //
    Stm32f4::timer2(Timer::CR1) = Stm32f4::timer2(Timer::CR1) | Timer::CR1_URS;     // only an under or overflow will trigger an IRQ
    Stm32f4::timer2(Timer::PSC) = uint16_t((clockSpeedMhz / 1000000) - 1);
    Stm32f4::timer2(Timer::EGR) = Stm32f4::timer2(Timer::EGR) | Timer::EGR_UG;

    Stm32f4::timer2(Timer::SR) = Stm32f4::timer2(Timer::SR) & ~Timer::SR_UIF;       // clear and enable the event
    Stm32f4::timer2(Timer::DIER) = Timer::DIER_UIE;

    // set the IRQ priority, hi --> low encoded as 0 --> 15 and enable the IRQ
    //
    Nvic::setPriority(F446IRQn::Timer2, priority);
    Nvic::enableIRQ(F446IRQn::Timer2);

    Stm32f4::timer2(Timer::CR1) = Stm32f4::timer2(Timer::CR1) | Timer::CR1_CEN;
}

uint64_t driver::Time::getMicros64()
{
    const volatile uint32_t firstRead = Stm32f4::timer2(Timer::CNT);
    uint64_t time = (uint64_t(overflowCount) << 32) | uint64_t(firstRead);

    // detect and handle if invoked at the same time as a timer rollover IRQ
    //
    const volatile uint32_t secondRead = Stm32f4::timer2(Timer::CNT);
    if (firstRead > secondRead) time = (uint64_t(overflowCount) << 32) | uint64_t(secondRead);

    return time;
}

uint32_t driver::Time::getMillis32()
{
    return getMicros64() / uint32_t(1000);
}

void driver::Time::spinWait(const uint64_t waitTimeUs)
{
    const auto now = getMicros64();
    while (getMicros64() <= now + waitTimeUs) asm volatile ("nop");
}

__attribute__((flatten, hot)) void Timer2IRQ::handler()
{
    // notes 1, always clear using the ~mask with rc_w0 bits!
    //       2, to avoid getting a double IRQ make sure that the SR gets sufficient cycles to clear
    //          as the the timer is clocking at a lower rate than the NVIC and this handler doesn't do much
    //       3, the code below is fragile, don't swap the statements otherwise #2 gets invalidated
    //
    Stm32f4::timer2(Timer::SR) = ~Timer::SR_UIF;
    driver::Time::overflowCount = driver::Time::overflowCount + 1;
}
