//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_RC_DECODER_H
#define BPL_RC_DECODER_H

#include <cstdint>

#include "framework/radiocontrol/srxl_status.hpp"
#include "framework/radiocontrol/srxl_statistics.hpp"

namespace bpl
{
    class RcDecoder
    {
        public:
            constexpr static uint32_t MAX_NUMBER_OF_CHANNELS = 32;

            virtual bpl::SrxlStatus decode() = 0;
            virtual int32_t getChannel(const uint32_t channel) const = 0;

            virtual bpl::SrxlStatus getLastStatus() const = 0;
            virtual bpl::SrxlStatistics getStatistics() const = 0;
    };
}

#endif
