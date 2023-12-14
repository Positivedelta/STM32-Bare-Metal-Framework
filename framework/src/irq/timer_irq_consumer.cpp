//
// (c) Bit Parallel Ltd, October 2023
//

#include "framework/irq/timer_irq_consumer.hpp"

bpl::TimerIrqConsumer::TimerIrqConsumer():
    count(0) {
}

void bpl::TimerIrqConsumer::tryFire(const uint32_t timeIncrement)
{
    count = count + timeIncrement;
    if (count >= getIrqRate())
    {
        count = 0;
        irq();
    }
}
