//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_SYSTICK_HANDLER_H
#define BPL_SYSTICK_HANDLER_H

#include <cstdint>

#include "irq_listeners.hpp"

class SysTick
{
    private:
        static inline uint32_t timeIncrement;
        static inline bpl::IrqListeners listeners;

    public:
        SysTick(const SysTick(const int32_t timeBase, bpl::IrqListeners&& irqListeners, const uint8_t priority)&) = delete;
        SysTick& operator=(const SysTick&) = delete;
        void operator=(const SysTick(const int32_t timeBase, bpl::IrqListeners&& irqListeners, const uint8_t priority)&) = delete;

        static SysTick& getInstance(const int32_t timeBase, bpl::IrqListeners&& irqListeners, const uint8_t priority);
        static void handler();

        void enable();
        void disable();

    private:
        SysTick(const int32_t timeBase, bpl::IrqListeners&& irqListeners, const uint8_t priority);
};

#endif
