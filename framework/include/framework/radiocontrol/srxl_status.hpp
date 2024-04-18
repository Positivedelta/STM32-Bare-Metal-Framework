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
            bool newData, staleData, headerError, crcError;
            uint32_t newDataCount, staleDataCount, headerErrorCount, crcErrorCount;

        public:
            SrxlStatus();
            void clearFlags();
            void indicateNewData();
            void indicateStaleData();
            void indicateHeaderError();
            void indicateCrcError();

            bool hasNewData() const;
            bool hasStaleData() const;
            bool hasHeaderError() const;
            bool hasCrcError() const;
            uint32_t getNewDataCount() const;
            uint32_t getStaleDataCount() const;
            uint32_t getHeaderErrorCount() const;
            uint32_t getCrcErrorCount() const;
    };
}

#endif
