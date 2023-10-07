//
// (c) Bit Parallel Ltd, October 2023
//

#include "irq_consumer.hpp"

bpl::IrqConsumer::IrqConsumer():
    count(0) {
}

void bpl::IrqConsumer::tryFire(const uint32_t timeIncrement)
{
    count = count + timeIncrement;
    if (count >= getIrqRate())
    {
        count = 0;
        irq();
    }
}
