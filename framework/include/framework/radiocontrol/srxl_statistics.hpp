//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_SRXL_STATISTICS_H
#define BPL_SRXL_STATISTICS_H

#include <cstdint>

namespace bpl
{
    class SrxlStatistics
    {
        private:
            uint32_t newDataCount, staleDataCount, failsafeConditionCount, packetHeaderErrorCount, packetTypeErrorCount, crcErrorCount;

        public:
            SrxlStatistics();

            void incrementNewDataCount();
            void incrementStaleDataCount();
            void incrementFailsafeConditionCount();
            void incrementPacketHeaderErrorCount();
            void incrementPacketTypeErrorCount();
            void incrementCrcErrorCount();

            uint32_t getNewDataCount() const;
            uint32_t getStaleDataCount() const;
            uint32_t getFailsafeConditionCount() const;
            uint32_t getPacketHeaderErrorCount() const;
            uint32_t getPacketTypeErrorCount() const;
            uint32_t getCrcErrorCount() const;
    };
}

#endif
