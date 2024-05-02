//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_RC_INPUT_STATISTICS_H
#define BPL_RC_INPUT_STATISTICS_H

#include <cstdint>

namespace bpl
{
    class RcInputStatistics
    {
        public:
            constexpr static uint32_t PACKET_RATE_TIMEOUT_US = 500'000;

        private:
            uint32_t packetRate, newDataCount, staleDataCount, failsafeConditionCount, packetErrorCount, crcErrorCount, parityErrorCount;

        public:
            RcInputStatistics();

            void setPacketRate(const uint32_t rate);
            void incrementNewDataCount();
            void incrementStaleDataCount();
            void incrementFailsafeConditionCount();
            void incrementPacketErrorCount();
            void incrementCrcErrorCount();
            void incrementParityErrorCount();

            uint32_t getPacketRate() const;
            uint32_t getNewDataCount() const;
            uint32_t getStaleDataCount() const;
            uint32_t getFailsafeConditionCount() const;
            uint32_t getPacketErrorCount() const;
            uint32_t getCrcErrorCount() const;
            uint32_t getParityErrorCount() const;
    };
}

#endif
