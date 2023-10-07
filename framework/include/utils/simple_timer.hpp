//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_SIMPLE_TIMER_H
#define BPL_SIMPLE_TIMER_H

#include <cstdint>

#include "irq_consumer.hpp"
#include "simple_callback.hpp"

namespace bpl
{
    class SimpleTimer : public IrqConsumer
    {
        private:
            const uint32_t period;
            SimpleCallBack& callBack;

        public:
            SimpleTimer(const uint32_t period, SimpleCallBack& callBack);
            void irq() override;
            uint32_t getIrqRate() const override;
    };
}

#endif
