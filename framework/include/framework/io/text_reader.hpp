//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_TEXT_READER_H
#define BPL_TEXT_READER_H

#include <cstdint>
#include <memory_resource>
#include <string>

#include "framework/io/ascii.hpp"
#include "framework/io/input_stream.hpp"
#include "framework/io/output_stream.hpp"
#include "framework/io/editproviders/input_edit_provider.hpp"
#include "framework/io/editproviders/input_prompt.hpp"

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

            // consume the contents of the input buffer and return true if the last character matches the keyCode
            //
            const bool isKey(uint8_t keyCode) const;

            // the use of std::prm::string will incur PMR memory allocation
            // lines can be edited using the left / right cursor keys in conjunction with the backspace and DEL keys
            //
            const std::pmr::string readln(const bpl::InputPrompt& prompt = bpl::InputPrompt()) const;

            // the use of std::prm::string will incur PMR memory allocation
            // lines can be edited using the left / right cursor keys in conjunction with the backspace and DEL keys
            // supports input history using the up / down cursor keys if an appropriate provider argument is supplied
            //
            const std::pmr::string readln(bpl::InputEditProvider<std::pmr::string>& editProvider, const bpl::InputPrompt& prompt = bpl::InputPrompt()) const;

            // FIXME! 1, update this version to allow editing with the cursor keys (as per the std::pmr::string version)
            //        2, implement a command history provider
            //
            // for the usual C++ reasons, the templated version of readln() is declared here in the .hpp file
            // if the buffer is filled before CR is pressed the input will be truncated
            // the output will always be 0x00 terminated
            // implements CR, BS and DEL handling
            //
            template<size_t maxSize>
            const size_t readln(bpl::InputEditProvider<char*>& editProvider, const bpl::InputPrompt& prompt = bpl::InputPrompt()) const
            {
                uint8_t byte;
                auto& text = editProvider.emptyBuffer();

                size_t i = 0;
                auto keepReading = true;
                while (keepReading)
                {
                    if (read(byte))
                    {
                        if (echo && (byte != bpl::Ascii::BS) && (byte != bpl::Ascii::BS_AS_DEL))
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

                            case bpl::Ascii::BS_AS_DEL:
                            {
                                if (i > 0)
                                {
                                    --i;
                                    if (echo) outputStream.write(bpl::Ascii::BS_AS_DEL);
                                }

                                break;
                            }

                            // don't allow the buffer to overflow, just truncate the input
                            //
                            default:
                                (i < maxSize) ? text[i++] = byte : --i;
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
