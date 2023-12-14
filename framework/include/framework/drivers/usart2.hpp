//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_STM32F4_USART2_DRIVER_H
#define BPL_STM32F4_USART2_DRIVER_H

#include <cstdint>
#include <functional>

#include "framework/io/byte_listener.hpp"
#include "framework/io/baud_rate.hpp"
#include "framework/io/input_stream.hpp"
#include "framework/io/output_stream.hpp"

class Usart2IRQ
{
    public:
        static void handler();
};

namespace driver
{
    class Usart2
    {
        friend void ::Usart2IRQ::handler();

        // note, buffer sizes must be a power of 2
        //
        private:
            inline constexpr static uint32_t IN_BUFFER_SIZE = 256, OUT_BUFFER_SIZE = 256;

            inline const static bpl::ByteListener nullListener = [](const uint8_t rxedByte) {
                //
                // benign, if needed can be replaced using In::setByteListener()
                //
            };

            volatile uint8_t inBuffer[IN_BUFFER_SIZE], outBuffer[OUT_BUFFER_SIZE];
            volatile uint32_t inBufferHead, inBufferTail;
            volatile uint32_t outBufferHead, outBufferTail;
            std::reference_wrapper<bpl::ByteListener> wrappedListener;

        public:
            class In : public bpl::InputStream
            {
                public:
                    In(Usart2& usart2);
                    const bool read(uint8_t& byte) const override;

                    // note, any registered listener must be quick as it will execute in the context of the IRQ handler
                    //
                    void setByteListener(const bpl::ByteListener& listener) const override;

                private:
                    Usart2& usart2;
            };

            class Out : public bpl::OutputStream
            {
                public:
                    Out(Usart2& usart2);
                    const bool write(const uint8_t byte) const override;
                    const uint32_t write(const uint8_t bytes[], const uint32_t length) const override;

                private:
                    const bool bufferFull() const;

                private:
                    Usart2& usart2;
            };

            static Usart2& getInstance();

            Usart2& initialise(const bpl::BaudRate& baudRate, const uint8_t priority);
            const bpl::InputStream& getInputStream() const;
            const bpl::OutputStream& getOutputStream() const;

        private:
            const In inputStream;
            const Out outputStream;

        private:
            Usart2();

        public:
            Usart2(const Usart2&) = delete;
            Usart2(const Usart2()&) = delete;
            Usart2& operator=(const Usart2&) = delete;
            void operator=(const Usart2()&) = delete;

            Usart2(Usart2&) = delete;
            Usart2(Usart2()&) = delete;
            Usart2& operator=(Usart2&) = delete;
            void operator=(Usart2()&) = delete;
    };
}

#endif
