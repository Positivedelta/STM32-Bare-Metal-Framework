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

            //note, for the usual C++ reasons, the templated version of readln() has to be declared here in the .hpp file
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
                        if (byte == bpl::Ascii::CR)
                        {
                            // notes 1, make the end of the c_str
                            //       2, don't include the 0x00 in the returned length, i.e. no i++
                            //
                            text[i] = 0x00;
                            break;
                        }

                        text[i++] = byte;
                    }
                    else
                    {
                        asm volatile ("wfi");
                    }
                }

                return i;
            }
    };
}

#endif
