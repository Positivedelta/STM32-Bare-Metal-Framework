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
#include "framework/radiocontrol/rc_input.hpp"

//
// FIXME! currently this class is based around the AR7700 receiver, it needs to be generic
//
// the default DX7 G2 channel index numbers
//   0: throttle, 1: aileron, 2: elevator, 3: rudder, 4: gear, 5: aux1 (pitch), 6: aux2
//

namespace bpl
{
    class SpektrumSrxlDecoder : public RcDecoder
    {
        private:
            constexpr static uint16_t CRC_POLYNOMINAL = 0x1021;
            constexpr static bool CRC_BIT_REVERSE = false;

            // spektrum SRXL receivers support 5.5, 11 and 22 millisecond packet frame times, this value is chosen to be universal
            //
            constexpr static uint64_t PACKET_TIMEOUT_US = 4'000;

            // FIXME! AR7700 specific constants
            //        1, check the channel data min, neutral and max values, some report these as 0x0002, 0x03fe, 0x7fe
            //        2, the newer 0xa6 packets use 16 bit channel values
            //
            constexpr static uint8_t SRXL_A5_BUFFER_SIZE = 18;
            constexpr static int32_t SRXL_A5_CHANNEL_RESOLUTION_BITS = 11;
            constexpr static int32_t SRXL_A5_CHANNEL_SCALE_BITS = bpl::RcInput::CHANNEL_RESOLUTION_BITS - SRXL_A5_CHANNEL_RESOLUTION_BITS;
            constexpr static uint32_t THROTTLE_CHANNEL = 0;

            driver::Uart& uart;
            driver::Time& time;
            std::array<uint8_t, SRXL_A5_BUFFER_SIZE> buffer;
            uint64_t lastTimestamp, lastCaptureTime;
            bpl::CcittCrc16<SRXL_A5_BUFFER_SIZE, CRC_POLYNOMINAL, CRC_BIT_REVERSE> crc;
            uint32_t bufferIndex;
            bool processed;
            bpl::RcInputStatus status;
            bpl::RcInputStatistics statistics;
            std::array<int32_t, bpl::RcInput::MAX_NUMBER_OF_CHANNELS> channels;

        public:
            SpektrumSrxlDecoder(driver::Uart& uart, driver::Time& time);

            // the main RC channels are signed and returned in the range [-4095, 4095], as defined in bpl::RcInput
            // note, the throttle channel is returned in the range [0..4095]
            //
            bpl::RcInputStatus decode() override;
            int32_t getChannel(const uint32_t channel) const override;

            bpl::RcInputStatus getStatus() const override;
            bpl::RcInputStatistics getStatistics() const override;
    };
}

#endif
