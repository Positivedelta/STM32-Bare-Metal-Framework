//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_IRQ_CONSUMER_H
#define BPL_IRQ_CONSUMER_H

#include <cstdint>

namespace bpl
{
    class IrqConsumer
    {
        private:
            uint32_t count;

        public:
            IrqConsumer();

            virtual void irq() = 0;
            virtual uint32_t getIrqRate() const = 0;

            virtual void tryFire(const uint32_t timeIncrement);
    };
}

#endif
