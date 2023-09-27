//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_SYSTICK_HANDLER_H
#define BPL_SYSTICK_HANDLER_H

#include "irq_listeners.hpp"

class SysTick
{
    private:
        static inline bpl::IrqListeners listeners;

    public:
        static void handler();

        SysTick(bpl::IrqListeners&& irqListeners);
        void enable();
        void disable();
};

#endif
