//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_SRXL_STATUS_H
#define BPL_SRXL_STATUS_H

#include <cstdint>

namespace bpl
{
    class SrxlStatus
    {
        private:
            bool newData, staleData, failsafeCondition, packetHeaderError, packetTypeError, crcError;

        public:
            SrxlStatus();
            void clearFlags();

            void indicateNewData();
            void indicateStaleData();
            void indicateFailsafeCondition();
            void indicatePacketHeaderError();
            void indicatePacketTypeError();
            void indicateCrcError();

            bool hasNewData() const;
            bool hasStaleData() const;
            bool hasFailsafeCondition() const;
            bool hasPacketHeaderError() const;
            bool hasPacketTypeError() const;
            bool hasCrcError() const;
    };
}

#endif
