//
// (c) Bit Parallel Ltd, 2021
//

#ifndef SYSTICK_DELAY_HANDLER_H
#define SYSTICK_DELAY_HANDLER_H

#include <cstdint>

class SysTick
{
    private:
        inline static volatile uint32_t ticks = 0;
        const uint32_t delayMillis;

    public:
        static void handler();

        SysTick(const uint32_t delayMillis);
        void delay();
};

#endif
