//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_SPEKTRUM_SRXL_DECODER_H
#define BPL_SPEKTRUM_SRXL_DECODER_H

#include <array>
#include <cstdint>

#include "framework/data/ccitt_crc_16.hpp"
#include "framework/drivers/time.hpp"
#include "framework/drivers/uart.hpp"
#include "framework/radiocontrol/srxl_status.hpp"
#include "framework/radiocontrol/srxl_statistics.hpp"

//
// FIXME! currently this class is based around the AR7700 receiver, it needs to be generic...
//

namespace bpl
{
    class SpektrumSrxlDecoder
    {
        private:
            constexpr static uint8_t BUFFER_SIZE = 18;
            constexpr static uint64_t PACKET_TIMEOUT_US = 4'000;
            constexpr static uint32_t MAX_CHANNELS = 32;
            constexpr static uint32_t NEUTRAL_CHANNEL_VALUE = 0x3ff;    // FIXME! AR7700 specific, the newer 0xa6 packets use 16 bit values

            driver::Uart& uart;
            driver::Time& time;
            std::array<uint8_t, BUFFER_SIZE> buffer;
            uint64_t lastTimestamp;
            bpl::CcittCrc16<BUFFER_SIZE, 0x1021, false> crc;            // xmodem (not bit reversed)
            uint32_t index;
            bool processed;
            bpl::SrxlStatus status;
            bpl::SrxlStatistics statistics;
            std::array<uint32_t, MAX_CHANNELS> channels;

        public:
            SpektrumSrxlDecoder(driver::Uart& uart, driver::Time& time);

            bpl::SrxlStatus decode();
            uint32_t getChannel(const uint32_t channel) const;

            bpl::SrxlStatus getLastStatus() const;
            bpl::SrxlStatistics getStatistics() const;
    };
}

#endif
