//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_FLIGHT_CONTROLLER_PENDSV_HANDLER_H
#define BPL_FLIGHT_CONTROLLER_PENDSV_HANDLER_H

#include <cstdint>
#include <functional>

#include "simple_irq_consumer.hpp"

class PendSV
{
    public:
        using Listener =  std::reference_wrapper<bpl::SimpleIrqConsumer>;

    private:
        class NullConsumer : public bpl::SimpleIrqConsumer
        {
            public:
                void irq() override
                {
                }
        };

        static inline bpl::SimpleIrqConsumer&& nullConsumer = NullConsumer();
        static inline Listener listener = std::ref(nullConsumer);

    public:
        PendSV(const PendSV(Listener&& irqListener, const uint8_t priority)&) = delete;
        PendSV& operator=(const PendSV&) = delete;
        void operator=(const PendSV(Listener&& irqListener, const uint8_t priority)&) = delete;

        static PendSV& getInstance(Listener&& irqListener, const uint8_t priority);
        static void handler();

        void enable();
        void disable();

    private:
        PendSV(Listener&& irqListener, const uint8_t priority);
};

#endif
