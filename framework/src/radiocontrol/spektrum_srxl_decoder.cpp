//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/io/byte_listener.hpp"
#include "framework/radiocontrol/spektrum_srxl_decoder.hpp"

bpl::SpektrumSrxlDecoder::SpektrumSrxlDecoder(driver::Uart& uart, driver::Time& time, const bool xPlusSupport):
    uart(uart), time(time), xPlusSupport(xPlusSupport), lastTimestamp(time.getMicros64()), lastCaptureTime(lastTimestamp),
    crc(bpl::CcittCrc16<SRXL_A5_BUFFER_SIZE, 0x1021, false>()), bufferIndex(0), processed(true), status(bpl::RcInputStatus()), statistics(bpl::RcInputStatistics()) {
        const bpl::ByteListener srxlHandler = [&](const uint8_t byte) {
            if (processed)
            {
                const auto now = time.getMicros64();
                if ((now - lastTimestamp) > PACKET_TIMEOUT_US)
                {
                    buffer[0] = byte;
                    bufferIndex = 1;
                }
                else
                {
                    buffer[bufferIndex++] = byte;
                    if (bufferIndex == SRXL_A5_BUFFER_SIZE)
                    {
                        // the earlier 'now' timestamp will be close enough...
                        //
                        statistics.setPacketRate(static_cast<uint32_t>(now - lastCaptureTime));
                        lastCaptureTime = now;

                        bufferIndex = 0;
                        processed = false;
                    }
                }

                lastTimestamp = now;
            }

            return true;
        };

        uart.setByteListener(srxlHandler);
        channels.fill(0);
}

bpl::RcInputStatus bpl::SpektrumSrxlDecoder::decode()
{
    status.clearFlags();

    if (!processed)
    {
        const auto packetHeader = buffer[0];
        if (packetHeader == 0xa5)
        {
            if (crc.compute(buffer) == 0)
            {
                // packet markers for the 0xa2 / 0xa5 packets
                //
                // [0x00] valid 1st data packet with fail safe condition
                // [0x01] valid 2nd data packet with fail safe condition
                // [0x02] valid and live 1st data packet
                // [0x03] valid and live 2nd data packet
                // [other] an invalid packet
                //
                // FIXME! the following decode has only been tested using a DX7 G2
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
                    //
                    for (auto index = 2; index < 14; index += 2)
                    {
                        // msb/lsb: on the 0xa2 / 0xa5 msb bits 6:3 are the channel number, bit 7 is not used
                        //          bits 2:0/7:0 represent the 11 bit channel data
                        //
                        const auto msb = buffer[index];
                        const auto channel = (msb & 0b0111'1000) >> 3;
                        auto value = (int32_t(msb & 0b0000'0111) << 8) | buffer[index + 1];

                        // scale the raw 11 bit values to the signed range [-4095, 4095], as defined in bpl::RcInput
                        // note, the throttle channel is scaled to the range [0..4095]
                        //
                        if (channel == THROTTLE_CHANNEL)
                        {
                            channels[channel] = value << (SRXL_A5_CHANNEL_SCALE_BITS - 1);
                        }
                        else
                        {
                            channels[channel] = (value << SRXL_A5_CHANNEL_SCALE_BITS) - (((1 << SRXL_A5_CHANNEL_RESOLUTION_BITS) - 1) << 1);
                        }
                    }

                    // FIXME! 1, on a DX7 G2 it the x-plus channel number is not fixed at 12, it varies, undefined data?
                    //        2, perhaps it only works on an x-plus capable transmitter, investigate...
                    //
                    // handle the xplus channels, one per frame, if enabled and available
                    // the channel number offset and data is always delivered via channel 12
                    //
                    if (xPlusSupport & ((buffer[14] & 0b0111'1000) == 0b0'1100'000))
                    {
                        // decode the 9 bit x-plus channel, for this frame, as stored in bytes [14, 15]
                        //
                        // format: [0 bbbb cc d] [dddddddd]
                        // bbbb: the channel number, this is always at 12
                        //   cc: channel number, low 2 bits, bit 2 is defined by bit 0 of the packet marker, e.g. 0 for '0x02', 1 for '0x03'
                        // d..d: channel data, 9 bits, with 256 as its midpoint
                        //
                        const auto xPlusChannel = 12 + (((packetMarker & 0b0000'0001) << 2) | ((buffer[14] & 0b0000'0110) >> 1));
                        const auto xPlusValue = (int32_t(buffer[14] & 0b0000'0001) << 8) | buffer[15];

                        // scaled to -4095..4095
                        //
                        constexpr auto xPlusOffset = 1 << (SRXL_A5_XPLUS_CHANNEL_RESOLUTION_BITS - 1);
                        channels[xPlusChannel] = 1 + ((xPlusValue - xPlusOffset) << SRXL_A5_XPLUS_CHANNEL_SCALE_BITS);
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
            // FIXME! to cope with newer receivers, [0xa6, 0xcd...] packets must be handled, these can be between 5 and 80 bytes long
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

        // has the RX stopped sending packets?
        //
        if ((time.getMicros64() - lastTimestamp) > bpl::RcInputStatistics::PACKET_RATE_TIMEOUT_US) statistics.setPacketRate(0);
    }

    return status;
}

int32_t bpl::SpektrumSrxlDecoder::getChannel(const uint32_t channel) const
{
    // FIXME! report / log this condition? add it to the SrxlStatistics error counts? return an optional? is it worth it?
    //
    if (channel >= bpl::RcInput::MAX_NUMBER_OF_CHANNELS) return 0;

    return channels[channel];
}

void bpl::SpektrumSrxlDecoder::enableXPlus(const bool enable)
{
    xPlusSupport = enable;
}

bool bpl::SpektrumSrxlDecoder::hasXPlusSupport() const
{
    return xPlusSupport;
}

bpl::RcInputStatus bpl::SpektrumSrxlDecoder::getStatus() const
{
    return status;
}

bpl::RcInputStatistics bpl::SpektrumSrxlDecoder::getStatistics() const
{
    return statistics;
}
