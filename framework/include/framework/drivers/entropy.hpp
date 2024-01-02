//
// (c) Bit Parallel Ltd, January 2024
//

#ifndef BPL_STM32F4_DRIVER_ENTROPY_H
#define BPL_STM32F4_DRIVER_ENTROPY_H

#include <cstdint>
#include <limits>

class RtcWkupIRQ
{
    public:
        static void handler();
};

//
// notes 1, uses timer 3, running as fast as possible and gets sampled by the RTC wakeup IRQ
//       2, the RTC wakeup timer is clocked at 16kHz with a period of 62.5us and has a reload value of 160
//       3, it will generate an IRQ every 10ms
//       4, the entropy pool can hold 8 32-bit values and is updated 4 bits at a time, i.e. it holds 64 4-bit chunks
//       5, it will take ~640ms to completely fill
//       6, a single random value will be available in ~80ms
//

namespace driver
{
    class Entropy
    {
        friend void ::RtcWkupIRQ::handler();

        private:
            static inline const int32_t POOL_SIZE = 8;
            static inline uint32_t MAX_UINT32 = std::numeric_limits<uint32_t>::max();

            class EntropyCache
            {
                public:
                    const uint8_t numberOfChunks;
                    volatile uint8_t head, tail;
                    volatile uint8_t count;
                    uint32_t pool[POOL_SIZE];

                    // note, the chunk count is computed as the POOL_SIZE in bits / nibble size, i.e. (POOL_SIZE * 32) / 4
                    //
                    EntropyCache():
                        numberOfChunks(POOL_SIZE << 3), head(0), tail(0), count(0) {
                    }
            };

            volatile EntropyCache cache;

        public:
            static Entropy& getInstance();

            Entropy(const Entropy()&) = delete;
            Entropy& operator=(const Entropy&) = delete;
            void operator=(const Entropy()&) = delete;

            Entropy& enable(const uint32_t priority, const bool mustFill = true);
            bool getRandom(uint32_t& value);
            bool getBoundedRandom(uint32_t& value, const uint32_t upperBound);
            bool getUniformBoundedRandom(uint32_t& value, const uint32_t upperBound);
            bool isEmpty() const;
            bool isFull() const;

        private:
            Entropy();
            void incrementIndex(volatile uint8_t& index);
            void appendToPool(uint32_t value);
    };
}

#endif
