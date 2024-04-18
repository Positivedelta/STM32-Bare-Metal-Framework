//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_SPEKTRUM_SRXL_DECODER_H
#define BPL_SPEKTRUM_SRXL_DECODER_H

#include <array>
#include <cstdint>

#include "framework/drivers/time.hpp"
#include "framework/drivers/uart.hpp"
#include "framework/radiocontrol/srxl_status.hpp"

//
// FIXME! currently this class is based around the AR7700 receiver, it needs to be generic...
//

namespace bpl
{
    class SpektrumSrxlDecoder
    {
        private:
            constexpr static uint8_t BUFFER_SIZE = 18;
            constexpr static uint64_t PACKET_TIMEOUT = 4'000;

            driver::Uart& uart;
            driver::Time& time;
            std::array<uint8_t, BUFFER_SIZE> buffer;
            uint64_t lastTimestamp;
            uint32_t index;
            bool processed;
            bpl::SrxlStatus status;

        public:
            SpektrumSrxlDecoder(driver::Uart& uart, driver::Time& time);
            bpl::SrxlStatus decode();
            int32_t getChannel(const int32_t channel) const;

        // FIXME! pass this in as a Crc type
        //
        private:
            bool checkCrc(std::array<uint8_t, BUFFER_SIZE> packet) const;
    };
}

#endif
