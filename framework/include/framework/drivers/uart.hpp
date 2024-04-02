//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_STM32F4_UART_DRIVER_BASE_H
#define BPL_STM32F4_UART_DRIVER_BASE_H

#include <cstdint>
#include <functional>

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
            inline const static bpl::ByteListener nullListener = [](const uint8_t byte) {
                //
                // benign, if needed can be replaced using In::setByteListener()
                //
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
    };
}

#endif
