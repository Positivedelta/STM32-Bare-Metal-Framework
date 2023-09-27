//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_IRQ_CONSUMER_H
#define BPL_IRQ_CONSUMER_H

namespace bpl
{
    class IrqConsumer
    {
        public:
            virtual void irq() = 0;
    };
}

#endif
