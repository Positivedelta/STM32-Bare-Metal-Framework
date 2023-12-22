//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_TEXT_READER_H
#define BPL_TEXT_READER_H

#include <cstdint>
#include <cstring>
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

            // FIXME! implement a command history provider
            //
            // for the usual C++ reasons, the templated version of readln() is declared here in the .hpp file
            // if the buffer is filled before CR is pressed the input will be truncated
            // the output will always be 0x00 terminated
            // implements CR, BS and DEL handling
            //
            // note, specify (1 + maxSize) if you want to discount the 0x00 termination character
            //
            template<size_t maxSize>
            const size_t readln(bpl::InputEditProvider<char*>& editProvider, const bpl::InputPrompt& prompt = bpl::InputPrompt()) const
            {
                uint8_t byte;
                auto& line = editProvider.emptyBuffer();

                auto compositeKeyDetectionLatch = false;
                auto deleteKeyDetectionLatch = false;
                size_t cursorPosition = 0;

                size_t size = 0;
                auto keepReading = true;
                while (keepReading)
                {
                    if (read(byte))
                    {
                        // handle the cursor keys
                        //
                        if ((!compositeKeyDetectionLatch) && (byte == bpl::Ascii::ESC))
                        {
                            compositeKeyDetectionLatch = true;
                            continue;
                        }

                        if (compositeKeyDetectionLatch)
                        {
                            if (byte == '[') continue;
                            compositeKeyDetectionLatch = false;

                            // up cursor, go back through the command history
                            //
                            if (byte == 'A')
                            {
/*                              if (editProvider.back())
                                {
                                    line = editProvider.buffer();
                                    if (echo)
                                    {
                                        outputStream.write(bpl::Ascii::ERASE_LINE, sizeof(bpl::Ascii::ERASE_LINE));
                                        prompt.display(outputStream);
                                        outputStream.write((uint8_t*)line.data(), line.size());
                                        cursorPosition = line.size();
                                    }
                                } */

                                continue;
                            }

                            // down cursor, move forwards through the command history
                            //
                            if (byte == 'B')
                            {
/*                              if (editProvider.forward())
                                {
                                    line = editProvider.buffer();
                                    if (echo)
                                    {
                                        outputStream.write(bpl::Ascii::ERASE_LINE, sizeof(bpl::Ascii::ERASE_LINE));
                                        prompt.display(outputStream);
                                        outputStream.write((uint8_t*)line.data(), line.size());
                                        cursorPosition = line.size();
                                    }
                                } */

                                continue;
                            }

                            // right cursor
                            //
                            if (byte == 'C')
                            {
                                if (cursorPosition == size) continue;

                                cursorPosition++;
                                if (echo) outputStream.write(bpl::Ascii::CURSOR_RIGHT, sizeof(bpl::Ascii::CURSOR_RIGHT));
                                continue;
                            }

                            // left cursor
                            //
                            if (byte == 'D')
                            {
                                if (cursorPosition == 0) continue;

                                cursorPosition--;
                                if (echo) outputStream.write(bpl::Ascii::CURSOR_LEFT, sizeof(bpl::Ascii::CURSOR_LEFT));
                                continue;
                            }

                            // note, ^[3 is the start of the DEL key sequence
                            //
                            if (byte == '3')
                            {
                                deleteKeyDetectionLatch = true;
                                continue;
                            }
                        }

                        // look for the remaining character of the DEL key sequence and then fake it as the delete character
                        //
                        if (deleteKeyDetectionLatch)
                        {
                            deleteKeyDetectionLatch = false;
                            if (byte == '~') byte = bpl::Ascii::BS_AS_DEL;
                        }

                        // echo the appended or inserted character
                        // notes 1, add the new character at the current cursor position and the cursor will advance
                        //       2, if this is an insert, overwite the existing characters starting from the insert index
                        //          also, save and restore the cursor back to the new insert point
                        //       3, don't allow the typed line to overflow its maximum length, inserts can truncate the typed line
                        //
                        if (echo && (byte != bpl::Ascii::BS) && (byte != bpl::Ascii::BS_AS_DEL))
                        {
                            if (byte == bpl::Ascii::CR)
                            {
                                outputStream.write(byte);
                                outputStream.write(bpl::Ascii::LF);
                            }
                            else
                            {
                                const auto fits = size < (maxSize - 1);
                                if (cursorPosition < size)
                                {
                                    outputStream.write(byte);
                                    outputStream.write(bpl::Ascii::SAVE_CURSOR, sizeof(bpl::Ascii::SAVE_CURSOR));

                                    const auto limit = (fits) ? size : size - 1;
                                    for (auto i = cursorPosition; i < limit; i++) outputStream.write(line[i]);
                                    outputStream.write(bpl::Ascii::RESTORE_CURSOR, sizeof(bpl::Ascii::RESTORE_CURSOR));
                                }
                                else
                                {
                                    if (fits) outputStream.write(byte);
                                }
                            }
                        }

                        switch (byte)
                        {
                            case bpl::Ascii::CR:
//                              editProvider.commit();      // FIXME! not needed until the bpl::CharArrayEditBufferWithHistory provider is ready
                                keepReading = false;
                                break;

                            case bpl::Ascii::BS:
                            case bpl::Ascii::BS_AS_DEL:
                            {
                                if ((cursorPosition > 0) && (size > 0))
                                {
                                    // using: std::memmove(void* dest, const void* src, std::size_t count) to shift down and overwrite the deleted character
                                    //
                                    if (cursorPosition < size) std::memmove(&line[cursorPosition - 1], &line[cursorPosition], size - cursorPosition);

                                    size--;
                                    cursorPosition--;
                                    if (echo)
                                    {
                                        outputStream.write(bpl::Ascii::BS_SAVE_CURSOR_ERASE_TO_EOL, sizeof(bpl::Ascii::BS_SAVE_CURSOR_ERASE_TO_EOL));
                                        for (size_t i = cursorPosition; i < size; i++) outputStream.write(line[i]);
                                        outputStream.write(bpl::Ascii::RESTORE_CURSOR, sizeof(bpl::Ascii::RESTORE_CURSOR));
                                    }
                                }

                                break;
                            }

                            default:
                            {
                                if (cursorPosition == size)
                                {
                                    // append, don't allow the buffer to overflow, just truncate the input
                                    //
                                    if (cursorPosition < (maxSize - 1))
                                    {
                                        line[cursorPosition] = byte;
                                        cursorPosition++;
                                        size++;
                                    }
                                }
                                else
                                {
                                    // insert, don't overflow the buffer, just lose characters from the end of the buffer
                                    // note, using: std::memmove(void* dest, const void* src, std::size_t count)
                                    //
                                    auto insertShift = size - cursorPosition;
                                    if ((cursorPosition + insertShift) == maxSize) insertShift--;
                                    std::memmove(&line[cursorPosition + 1], &line[cursorPosition], insertShift);

                                    line[cursorPosition] = byte;
                                    cursorPosition++;
                                    size++;
                                }

                                // too long, either make space for another insert (at the expense of the last character) or for the '0x00' termination byte
                                //
                                if (size == maxSize) size--;
                            }
                        }
                    }
                    else
                    {
                        asm volatile ("wfi");
                    }
                }

                // terminate to generate the equivalent of a c_str()
                // note, size has already been appropriately adjusted
                //
                line[size] = 0x00;

                return size;
            }
    };
}

#endif
