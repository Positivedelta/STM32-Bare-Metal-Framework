//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_SPEKTRUM_SRXL_DECODER_H
#define BPL_SPEKTRUM_SRXL_DECODER_H

#include <cstdint>
#include <functional>

#include "framework/drivers/time.hpp"
#include "framework/drivers/uart.hpp"

//
// FIXME! currently this class is based around the AR7700 receiver, it needs to be generic...
//

namespace bpl
{
    class SpektrumSrxlDecoder
    {
        private:
            Uart& uart;
            Time& time;

        public:
            SpektrumSrxlDecoder(driver::Uart& uart, driver::Time& time);
            int32_t getChannel(const int32_t channel);
    };
}

#endif
