//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_USART2_HANDLER_H
#define BPL_USART2_HANDLER_H

#include <cstdint>

#include "baud_rate.hpp"
#include "input_stream.hpp"
#include "output_stream.hpp"

class Usart2IRQ
{
    // note, buffer sizes must be a power of 2
    //
    private:
        inline constexpr static uint32_t IN_BUFFER_SIZE = 256, OUT_BUFFER_SIZE = 256;
        inline volatile static uint8_t inBuffer[IN_BUFFER_SIZE], outBuffer[OUT_BUFFER_SIZE];
        inline volatile static uint32_t inBufferHead = 0, inBufferTail = 0;
        inline volatile static uint32_t outBufferHead = 0, outBufferTail = 0;

        inline const static bpl::ByteListener defaultByteListener = [](const uint8_t rxedByte) {
            //
            // the default byte listener, benign and is intended to be replaced via In::setByteListener()
            //
        };

        inline static auto wrappedByteListener = std::ref(defaultByteListener);

    public:
        class In : public bpl::InputStream
        {
            public:
                const bool read(uint8_t& byte) const override;

                // note, any registered listener must be quick as it will execute in the context of the IRQ handler
                //
                void setByteListener(const bpl::ByteListener& byteListener) const override;
        };

        class Out : public bpl::OutputStream
        {
            public:
                const bool write(const uint8_t byte) const override;
                const uint32_t write(const uint8_t bytes[], const uint32_t length) const override;

            private:
                const bool bufferFull() const;
        };

        static Usart2IRQ& getInstance(const bpl::BaudRate& baudRate);
        static void handler();

        const bpl::BaudRate& baudRate;
        const bpl::InputStream& getInputStream();
        const bpl::OutputStream& getOutputStream();

    private:
        Usart2IRQ(const bpl::BaudRate& baudRate);
        const In inputStream;
        const Out outputStream;

    public:
//      Usart2IRQ(const Usart2IRQ&) = delete;           // FIXME! not 100% sure why is this a problem?
        Usart2IRQ& operator=(const Usart2IRQ&) = delete;
//      void operator=(const Usart2IRQ&) = delete;
};

#endif
