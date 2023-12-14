//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_SIMPLE_IRQ_CONSUMER_H
#define BPL_SIMPLE_IRQ_CONSUMER_H

namespace bpl
{
    class SimpleIrqConsumer
    {
        public:
            virtual void irq() = 0;
    };
}

#endif
