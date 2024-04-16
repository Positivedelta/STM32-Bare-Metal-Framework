//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_STM32F4_UART_DRIVER_BASE_H
#define BPL_STM32F4_UART_DRIVER_BASE_H

#include <cstdint>
#include <functional>

#include "framework/io/baud_rate.hpp"
#include "framework/io/byte_listener.hpp"
#include "framework/io/io_stream.hpp"
#include "framework/io/input_stream.hpp"
#include "framework/io/output_stream.hpp"

namespace driver
{
    using ScheduleTxProvider = std::function<void()>;

    class Uart : public bpl::IOStream
    {
        private:
            // benign, if needed can be replaced using In::setByteListener()
            // note, returns false to indicate that the byte has not been fully handled and that further action is likely to be required
            //
            inline const static bpl::ByteListener nullListener = [](const uint8_t byte) {
                return false;
            };

        // note, buffer sizes must be a power of 2
        //
        protected:
            inline constexpr static uint32_t IN_BUFFER_SIZE = 256, OUT_BUFFER_SIZE = 256;

            volatile uint8_t inBuffer[IN_BUFFER_SIZE], outBuffer[OUT_BUFFER_SIZE];
            volatile uint32_t inBufferHead, inBufferTail;
            volatile uint32_t outBufferHead, outBufferTail;
            bpl::ByteListener wrappedListener;

        public:
            class In : public bpl::InputStream
            {
                private:
                    Uart& uart;

                public:
                    In(Uart& uart):
                        uart(uart) {
                    }

                    bool read(uint8_t& byte) const override
                    {
                        if (uart.inBufferHead != uart.inBufferTail)
                        {
                            byte = uart.inBuffer[uart.inBufferTail & (IN_BUFFER_SIZE - 1)];
                            uart.inBufferTail = uart.inBufferTail + 1;

                            return true;
                        }

                        return false;
                    }

                    void setByteListener(const bpl::ByteListener& listener = nullListener) const override
                    {
                        uart.wrappedListener = listener;
                    }
            };

            class Out : public bpl::OutputStream
            {
                private:
                    Uart& uart;
                    const ScheduleTxProvider scheduleTx;

                public:
                    Out(Uart& uart, const auto& scheduleTx):
                        uart(uart), scheduleTx(std::move(scheduleTx)) {
                    }

                    bool write(const uint8_t byte) const override
                    {
                        if (bufferFull()) return false;

                        uart.outBuffer[uart.outBufferHead & (OUT_BUFFER_SIZE - 1)] = byte;
                        uart.outBufferHead = uart.outBufferHead + 1;

                        if (uart.outBufferHead - uart.outBufferTail == 1) scheduleTx();
                        return true;
                    }

                    uint32_t write(const uint8_t bytes[], const uint32_t length) const override
                    {
                        for (uint32_t i = 0; i < length; i++)
                        {
                            if (write(bytes[i]) == false) return i;
                        }

                        return length;
                    }

                private:
                    bool bufferFull() const
                    {
                        return (uart.outBufferHead - uart.outBufferTail) == OUT_BUFFER_SIZE;
                    }
            };

        private:
            const In inputStream;
            const Out outputStream;

        public:
            Uart(const auto& scheduleTx):
                inBufferHead(0), inBufferTail(0), outBufferHead(0), outBufferTail(0), wrappedListener(nullListener),
                inputStream(Uart::In(*this)), outputStream(Uart::Out(*this, scheduleTx)) {
            }

            const bpl::InputStream& getInputStream() const override
            {
                return inputStream;
            }

            const bpl::OutputStream& getOutputStream() const override
            {
                return outputStream;
            }

            //
            // baud rate calculation utility methods
            //

            // calculate the usart BRR baud rate register value when using a 45 MHz clock
            // notes 1, CR2 and CR3 are left at their default values (stop bits, flow control...)
            //       2, [mantissa.divisor] = clock / (oversample:16|8 * baudRate) with .xxxx|.xxx used for the divisor
            //
            static uint32_t baudAPB1(const bpl::BaudRate& baudRate)
            {
                uint32_t brrValue = 0;
                switch (baudRate)
                {
                    // 585.9375 (closest fraction), i.e. 585 | 15 (has a 0% error)
                    //
                    case bpl::BaudRate::BPS_4800:
                        brrValue = (585 << 4) | 15;
                        break;

                    // 293.0000 (closest fraction), i.e. 292 | 16 (has a 0.011% error)
                    //
                    case bpl::BaudRate::BPS_9600:
                        brrValue = (292 << 4) | 16;
                        break;

                    // 48.8125 (closest fraction), i.e. 48 | 13 (has a -0.032% error)
                    //
                    case bpl::BaudRate::BPS_57600:
                        brrValue = (48 << 4) | 13;
                        break;

                    // 24.4375 (closest fraction), i.e. 24 | 7 (has an 0.096% error)
                    //
                    case bpl::BaudRate::BPS_115200:
                        brrValue = (24 << 4) | 7;
                        break;

                    // 12.1875 (closest fraction), i.e. 12 | 3 (has an 0.16% error)
                    //
                    case bpl::BaudRate::BPS_230400:
                        brrValue = (12 << 4) | 3;
                        break;

                    // 6.125 (closest fraction), i.e. 6 | 2 (has an 0.351% error)
                    //
                    case bpl::BaudRate::BPS_460800:
                        brrValue = (6 << 4) | 2;
                        break;

                    // 3.0625 (closest fraction), i.e. 3 | 1 (has an 0.351% error)
                    //
                    case bpl::BaudRate::BPS_921600:
                        brrValue = (3 << 4) | 1;
                        break;
                }

                return brrValue;
            }

            // calculate the usart BRR baud rate register value when using a 90 MHz clock
            // notes 1, CR2 and CR3 are left at their default values (stop bits, flow control...)
            //       2, [mantissa.divisor] = clock / (oversample:16|8 * baudRate) with .xxxx|.xxx used for the divisor
            //
            static uint32_t baudAPB2(const bpl::BaudRate& baudRate)
            {
                uint32_t brrValue = 0;
                switch (baudRate)
                {
                    // 1171.875 (closest fraction), i.e. 1171 | 14 (has a 0% error)
                    //
                    case bpl::BaudRate::BPS_4800:
                        brrValue = (1171 << 4) | 14;
                        break;

                    // 585.9375 (closest fraction), i.e. 585 | 15 (has a 0.0% error)
                    //
                    case bpl::BaudRate::BPS_9600:
                        brrValue = (585 << 4) | 15;
                        break;

                    // 97.6875 (closest fraction), i.e. 97 | 11 (has a 0.032% error)
                    //
                    case bpl::BaudRate::BPS_57600:
                        brrValue = (97 << 4) | 11;
                        break;

                    // 48.8125 (closest fraction), i.e. 48 | 13 (has an -0.032% error)
                    //
                    case bpl::BaudRate::BPS_115200:
                        brrValue = (48 << 4) | 13;
                        break;

                    // 24.4375 (closest fraction), i.e. 24 | 7 (has an 0.096% error)
                    //
                    case bpl::BaudRate::BPS_230400:
                        brrValue = (24 << 4) | 7;
                        break;

                    // 12.1875 (closest fraction), i.e. 12 | 3 (has an -0.16% error)
                    //
                    case bpl::BaudRate::BPS_460800:
                        brrValue = (12 << 4) | 3;
                        break;

                    // 6.125 (closest fraction), i.e. 6 | 2 (has an 0.351% error)
                    //
                    case bpl::BaudRate::BPS_921600:
                        brrValue = (6 << 4) | 2;
                        break;
                }

                return brrValue;
            }
    };
}

#endif
