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
#include "framework/radiocontrol/rc_decoder.hpp"

//
// FIXME! currently this class is based around the AR7700 receiver, it needs to be generic...
//

namespace bpl
{
    class SpektrumSrxlDecoder : public RcDecoder
    {
        private:
            constexpr static uint8_t BUFFER_SIZE = 18;
            constexpr static uint64_t PACKET_TIMEOUT_US = 4'000;

            // FIXME! 1, AR7700 specific, the newer 0xa6 packets use 16 bit values
            //        2, check these values, some report these as 0x0002, 0x03fe, 0x7fe
            //
            constexpr static int32_t MIN_CHANNEL_VALUE = 0x0000;
            constexpr static int32_t NEUTRAL_CHANNEL_VALUE = 0x03ff;
            constexpr static int32_t MAX_CHANNEL_VALUE = 0x07ff;
            constexpr static int32_t CHANNEL_RANGE = MAX_CHANNEL_VALUE - MIN_CHANNEL_VALUE;
            constexpr static uint32_t THROTTLE_CHANNEL = 0;

            driver::Uart& uart;
            driver::Time& time;
            std::array<uint8_t, BUFFER_SIZE> buffer;
            uint64_t lastTimestamp;
            bpl::CcittCrc16<BUFFER_SIZE, 0x1021, false> crc;            // xmodem (not bit reversed)
            uint32_t bufferIndex;
            bool processed;
            bpl::RcInputStatus status;
            bpl::RcInputStatistics statistics;
            std::array<int32_t, MAX_NUMBER_OF_CHANNELS> channels;

        public:
            SpektrumSrxlDecoder(driver::Uart& uart, driver::Time& time);

            // the main RC channels are signed and returned in the range [-4096, 4096], as defined in bpl::RcInput
            // note, the throttle channel is returned in the range [0..4096]
            //
            bpl::RcInputStatus decode() override;
            int32_t getChannel(const uint32_t channel) const override;

            bpl::RcInputStatus getStatus() const override;
            bpl::RcInputStatistics getStatistics() const override;
    };
}

#endif
