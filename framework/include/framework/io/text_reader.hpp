//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_TEXT_READER_H
#define BPL_TEXT_READER_H

#include <cstdint>
#include <memory_resource>
#include <string>

#include "framework/utils/ascii.hpp"
#include "framework/io/input_stream.hpp"
#include "framework/io/output_stream.hpp"

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
            //       4, implements CR, BS and DEL handling
            //
            template<size_t n>
            const uint32_t readln(char(&text)[n]) const
            {
                uint8_t byte;
                uint32_t i = 0;
                auto keepReading = true;
                while (keepReading)
                {
                    if (read(byte))
                    {
                        if (echo && (byte != bpl::Ascii::BS) && (byte != bpl::Ascii::DEL))
                        {
                            outputStream.write(byte);
                            if (byte == bpl::Ascii::CR) outputStream.write(bpl::Ascii::LF);
                        }

                        switch (byte)
                        {
                            case bpl::Ascii::CR:
                                keepReading = false;
                                break;

                            case bpl::Ascii::BS:
                            {
                                if (i > 0)
                                {
                                    --i;
                                    if (echo)
                                    {
                                        const uint8_t action[] = {bpl::Ascii::BS, bpl::Ascii::SPACE, bpl::Ascii::BS};
                                        outputStream.write(action, sizeof(action));
                                    }
                                }

                                break;
                            }

                            case bpl::Ascii::DEL:
                            {
                                if (i > 0)
                                {
                                    --i;
                                    if (echo) outputStream.write(bpl::Ascii::DEL);
                                }

                                break;
                            }

                            // don't allow the buffer to overflow, just truncate the input
                            //
                            default:
                                (i < n) ? text[i++] = byte : --i;
                        }
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
