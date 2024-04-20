//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/io/byte_listener.hpp"
#include "framework/radiocontrol/spektrum_srxl_decoder.hpp"

bpl::SpektrumSrxlDecoder::SpektrumSrxlDecoder(driver::Uart& uart, driver::Time& time):
    uart(uart), time(time), lastTimestamp(time.getMicros64()), crc(bpl::CcittCrc16<BUFFER_SIZE, 0x1021, false>()), index(0), processed(true), status(bpl::SrxlStatus()) {
        const bpl::ByteListener uartHandler = [&](const uint8_t byte) {
            const auto now = time.getMicros64();
            if (processed)
            {
                if ((now - lastTimestamp) > PACKET_TIMEOUT)
                {
                    buffer[0] = byte;
                    index = 1;
                }
                else
                {
                    buffer[index++] = byte;
                    if (index == BUFFER_SIZE)
                    {
                        index = 0;
                        processed = false;
                    }
                }

                lastTimestamp = now;
            }

            return true;
        };

        uart.setByteListener(uartHandler);
}

bpl::SrxlStatus bpl::SpektrumSrxlDecoder::decode()
{
    status.clearFlags();
    if (!processed)
    {
        const auto header = buffer[0];
        if ((header == 0xa5) || (header == 0xa6))
        {
            if (crc.compute(buffer) == 0)
            {
                //
                // FIXME! do the decode...
                //

                status.indicateNewData();
            }
            else
            {
                status.indicateCrcError();
            }
        }
        else
        {
            status.indicateHeaderError();
        }

        processed = true;
    }
    else
    {
        status.indicateStaleData();
    }

    return status;
}

int32_t bpl::SpektrumSrxlDecoder::getChannel(const int32_t channel) const
{
    return 0;
}
