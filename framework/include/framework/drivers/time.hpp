//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_STM32F4_TIME_H
#define BPL_STM32F4_TIME_H

#include <cstdint>

//
// notes 1, bound to the 32-bit timer2
//       2, implemented as a singleton as there is only one instance of the associated hardware timer
//

class Timer2IRQ
{
    public:
        static void handler();
};

namespace driver
{
    class Time
    {
        friend void ::Timer2IRQ::handler();

        private:
            volatile static inline uint32_t overflowCount = 0;

        public:
            static Time& getInstance(const uint32_t clockSpeedMhz, const uint32_t priority);

            Time(const Time&) = delete;
            Time(const Time(const uint32_t clockSpeedMhz, const uint32_t priority)&) = delete;
            Time& operator=(const Time&) = delete;
            void operator=(const Time(const uint32_t clockSpeedMhz, const uint32_t priority)&) = delete;

            Time(Time&) = delete;
            Time(Time(const uint32_t clockSpeedMhz, const uint32_t priority)&) = delete;
            Time& operator=(Time&) = delete;
            void operator=(Time(const uint32_t clockSpeedMhz, const uint32_t priority)&) = delete;

            uint64_t getMicros64();
            uint32_t getMillis32();

            void spinWait(const uint64_t waitTimeUs);

        private:
            Time(const uint32_t clockSpeedMhz, const uint32_t priority);
    };
}

#endif
