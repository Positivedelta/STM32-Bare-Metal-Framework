//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_DELEGATED_FLASHER_SYSTICK_HANDLER_H
#define BPL_DELEGATED_FLASHER_SYSTICK_HANDLER_H

#include <cstdint>

#include "framework/irq/timer_irq_consumer.hpp"
#include "framework/irq/irq_listeners.hpp"

class SysTick
{
    using Listeners =  bpl::IrqListeners<bpl::TimerIrqConsumer>;

    private:
        static inline uint32_t timeIncrement;
        static inline Listeners listeners;

    public:
        SysTick(const SysTick(const uint32_t timeBase, Listeners&& irqListeners, const uint8_t priority)&) = delete;
        SysTick& operator=(const SysTick&) = delete;
        void operator=(const SysTick(const uint32_t timeBase, Listeners&& irqListeners, const uint8_t priority)&) = delete;

        static SysTick& getInstance(const uint32_t timeBase, Listeners&& irqListeners, const uint8_t priority);
        static void handler();

        void enable();
        void disable();

    private:
        SysTick(const uint32_t timeBase, Listeners&& irqListeners, const uint8_t priority);
};

#endif
