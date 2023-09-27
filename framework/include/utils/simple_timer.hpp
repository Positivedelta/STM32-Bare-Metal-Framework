//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_SIMPLE_TIMER_H
#define BPL_SIMPLE_TIMER_H

#include <cstdint>
#include <functional>

#include "irq_consumer.hpp"
#include "simple_callback.hpp"

namespace bpl
{
    class SimpleTimer : public IrqConsumer
    {
        private:
            const int32_t period;
            SimpleCallBack& callBack;
            int32_t counter;

        public:
            SimpleTimer(const int32_t period, SimpleCallBack& callBack);
            void irq() override;
    };
}

#endif
