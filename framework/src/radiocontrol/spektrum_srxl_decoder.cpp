//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/io/byte_listener.hpp"
#include "framework/radiocontrol/rc_input.hpp"
#include "framework/radiocontrol/spektrum_srxl_decoder.hpp"

bpl::SpektrumSrxlDecoder::SpektrumSrxlDecoder(driver::Uart& uart, driver::Time& time):
    uart(uart), time(time), lastTimestamp(time.getMicros64()), crc(bpl::CcittCrc16<BUFFER_SIZE, 0x1021, false>()), bufferIndex(0), processed(true),
    status(bpl::RcInputStatus()), statistics(bpl::RcInputStatistics()) {
        const bpl::ByteListener uartHandler = [&](const uint8_t byte) {
            const auto now = time.getMicros64();
            if (processed)
            {
                if ((now - lastTimestamp) > PACKET_TIMEOUT_US)
                {
                    buffer[0] = byte;
                    bufferIndex = 1;
                }
                else
                {
                    buffer[bufferIndex++] = byte;
                    if (bufferIndex == BUFFER_SIZE)
                    {
                        bufferIndex = 0;
                        processed = false;
                    }
                }

                lastTimestamp = now;
            }

            return true;
        };

        uart.setByteListener(uartHandler);
        channels.fill(0);
}

bpl::RcInputStatus bpl::SpektrumSrxlDecoder::decode()
{
    status.clearFlags();

    if (!processed)
    {
        // FIXME! to cope with newer receivers, [0xa6, 0xcd...] packets must be handled, these can be between 5 and 80 bytes long
        //
        const auto packetHeader = buffer[0];
        if (packetHeader == 0xa5)
        {
            if (crc.compute(buffer) == 0)
            {
                // packet markers for the AR7700 receiver
                //
                // [0x00] valid 1st data packet with fail safe condition
                // [0x01] valid 2nd data packet with fail safe condition
                // [0x02] valid and live 1st data packet
                // [0x03] valid and live 2nd data packet
                // [other] an invalid packet
                //
                // FIXME! the following decode is based on a DX7 G2, the 0xa2 and 0xa3 packet are the same
                //        although this may not be accurate... will depend on how CH7 is handled, but the first six channels should be the same in both packets
                //
                const auto packetMarker = buffer[1];
                if (packetMarker < 0x04)
                {
                    if (packetMarker < 0x02)
                    {
                        status.indicateFailsafeCondition();
                        statistics.incrementFailsafeConditionCount();
                    }

                    // do the decode, including possible failsafe values
                    // FIXME! for the moment, just first 6 channels... i.e. figure out how CH7 is handled
                    // FIXME! all of the code in the below loop is AR7700 specific
                    //
                    for (auto index = 2; index < 14; index += 2)
                    {
                        // msb/lsb: on the AR77000 msb bits 6:3 are the channel number, bit 7 is not used
                        //          bits 2:0/7:0 represent the 11 bit channel data
                        //
                        const auto msb = buffer[index];
                        const auto channel = (msb & 0b0111'1000) >> 3;
                        auto value = (int32_t(msb & 0b0000'0111) << 8) | buffer[index + 1];

                        // scale to signed values in the range [-4096, 4096], as defined in bpl::RcInput
                        // the throttle channel is scaled to the range [0..4096]
                        //
                        if (channel == THROTTLE_CHANNEL)
                        {
                            channels[channel] = ((MIN_CHANNEL_VALUE + value) / CHANNEL_RANGE) << bpl::RcInput::ABSOLUTE_MAX_CHANNEL_SHIFT;
                        }
                        else
                        {
                            channels[channel] = ((value - NEUTRAL_CHANNEL_VALUE) / CHANNEL_RANGE) << (bpl::RcInput::ABSOLUTE_MAX_CHANNEL_SHIFT + 1);
                        }
                    }

                    status.indicateNewData();
                    statistics.incrementNewDataCount();
                }
                else
                {
                    status.indicatePacketError();
                    statistics.incrementPacketErrorCount();
                }
            }
            else
            {
                status.indicateCrcError();
                statistics.incrementCrcErrorCount();
            }
        }
/*      else if (packetHeader == 0xa6)
        {
            //
            // notes 1, refer to the SRLX2 revision K documentation...
            //       2, buy an up-to-date RX to test with...
            //
        } */
        else
        {
            status.indicatePacketError();
            statistics.incrementPacketErrorCount();
        }

        processed = true;
    }
    else
    {
        status.indicateStaleData();
        statistics.incrementStaleDataCount();
    }

    return status;
}

int32_t bpl::SpektrumSrxlDecoder::getChannel(const uint32_t channel) const
{
    // FIXME! report / log this condition? add it to the SrxlStatistics error counts? return an optional? is it worth it?
    //
    if (channel >= MAX_NUMBER_OF_CHANNELS) return 0;

    return channels[channel];
}

bpl::RcInputStatus bpl::SpektrumSrxlDecoder::getStatus() const
{
    return status;
}

bpl::RcInputStatistics bpl::SpektrumSrxlDecoder::getStatistics() const
{
    return statistics;
}
