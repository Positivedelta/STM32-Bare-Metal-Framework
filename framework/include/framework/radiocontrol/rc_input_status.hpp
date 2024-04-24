//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_RC_INPUT_STATUS_H
#define BPL_RC_INPUT_STATUS_H

#include <cstdint>

namespace bpl
{
    class RcInputStatus
    {
        private:
            bool newData, staleData, failsafeCondition, packetError, crcError, parityError;

        public:
            RcInputStatus();
            void clearFlags();

            void indicateNewData();
            void indicateStaleData();
            void indicateFailsafeCondition();
            void indicatePacketError();
            void indicateCrcError();
            void indicateParityError();

            bool hasNewData() const;
            bool hasStaleData() const;
            bool hasFailsafeCondition() const;
            bool hasPacketError() const;
            bool hasCrcError() const;
            bool hasParityError() const;
    };
}

#endif
