//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_TEXT_READER_H
#define BPL_TEXT_READER_H

#include <cstdint>
#include <memory_resource>
#include <string>

#include "ascii.hpp"
#include "input_stream.hpp"
#include "output_stream.hpp"

namespace bpl
{
    class TextReader
    {
        private:
            const bpl::InputStream& inputStream;
            const bpl::OutputStream& outputStream;
            const bool echo;

        public:
            // include an OutputStream in order to echo RXed characters
            //
            TextReader(const bpl::InputStream& inputStream);
            TextReader(const bpl::InputStream& inputStream, const bpl::OutputStream& outputStream);

            const bool read(uint8_t& byte) const;

            // notes 1, the readln() methods return std::pmr::string or uint8_t[] values
            //       2, the use of std::prm::string will incur PMR memory allocation
            //
            const std::pmr::string readln() const;

            // notes 1, for the usual C++ reasons, the templated version of readln() has to be declared here in the .hpp file
            //       2, if the buffer is filled before CR is pressed the input will be truncated
            //       3, the output will always be 0x00 terminated and the returned length does not include the 0x00 byte
            //
            template<size_t n>
            const uint32_t readln(char(&text)[n]) const
            {
                uint8_t byte;
                uint32_t i = 0;
                while (true)
                {
                    if (read(byte))
                    {
                        if (byte == bpl::Ascii::CR) break;

                        text[i++] = byte;
                        if (i == n) break;
                    }
                    else
                    {
                        asm volatile ("wfi");
                    }
                }

                // terminate and generate the equivalent of a c_str()
                //
                text[i] = 0x00;

                return i;
            }
    };
}

#endif
