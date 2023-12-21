//
// (c) Bit Parallel Ltd, December 2023
//

#include "framework/io/text_reader.hpp"
#include "framework/io/null_output_stream.hpp"
#include "framework/io/editproviders/string_edit_buffer.hpp"

bpl::TextReader::TextReader(const InputStream& inputStream):
    inputStream(inputStream), outputStream(NullOutputStream()), echo(false) {
}

bpl::TextReader::TextReader(const bpl::InputStream& inputStream, const bpl::OutputStream& outputStream):
    inputStream(inputStream), outputStream(outputStream), echo(true) {
}

const bool bpl::TextReader::read(uint8_t& byte) const
{
    return inputStream.read(byte);
}

const bool bpl::TextReader::isKey(uint8_t keyCode) const
{
    bool pressed = false;

    uint8_t key;
    while (read(key)) pressed = true;

    return pressed && (key == keyCode);
}

// implements CR, BS and DEL handling
// supports editing using the LEFT / RIGHT cursor keys (no history)
//
const std::pmr::string bpl::TextReader::readln(const bpl::InputPrompt& prompt) const
{
    auto editBuffer = bpl::StringEditBuffer();
    return readln(editBuffer, prompt);
}

// implements CR, BS and DEL handling
// supports editing using the LEFT / RIGHT cursor keys
// uses the UP / DOWN cursor keys to navigate the command history
//
const std::pmr::string bpl::TextReader::readln(bpl::InputEditProvider<std::pmr::string>& editProvider, const bpl::InputPrompt& prompt) const
{
    uint8_t byte;
    auto& line = editProvider.emptyBuffer();

    auto compositeKeyDetectionLatch = false;
    auto deleteKeyDetectionLatch = false;
    uint32_t cursorPosition = 0;

    auto keepReading = true;
    while (keepReading)
    {
        if (read(byte))
        {
            // consume the cursor keys
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
                    if (editProvider.back())
                    {
                        line = editProvider.buffer();
                        if (echo)
                        {
                            outputStream.write(bpl::Ascii::ERASE_LINE, sizeof(bpl::Ascii::ERASE_LINE));
                            prompt.display(outputStream);
                            outputStream.write((uint8_t*)line.data(), line.size());
                            cursorPosition = line.size();
                        }
                    }

                    continue;
                }

                // down cursor, move forwards through the command history
                //
                if (byte == 'B')
                {
                    if (editProvider.forward())
                    {
                        line = editProvider.buffer();
                        if (echo)
                        {
                            outputStream.write(bpl::Ascii::ERASE_LINE, sizeof(bpl::Ascii::ERASE_LINE));
                            prompt.display(outputStream);
                            outputStream.write((uint8_t*)line.data(), line.size());
                            cursorPosition = line.size();
                        }
                    }

                    continue;
                }

                // right cursor
                //
                if (byte == 'C')
                {
                    if (cursorPosition == line.size()) continue;

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

            // echo the append or insert
            // notes 1, add the new character at the current cursor position and the cursor will advance
            //       2, if this is an insert, overwite the existing characters starting from the insert index
            //          also, save and restore the cursor back to the new insert point
            //
            if (echo && (byte != bpl::Ascii::BS) && (byte != bpl::Ascii::BS_AS_DEL))
            {
                outputStream.write(byte);
                if (byte == bpl::Ascii::CR)
                {
                    outputStream.write(bpl::Ascii::LF);
                }
                else
                {
                    if (cursorPosition < line.size())
                    {
                        outputStream.write(bpl::Ascii::SAVE_CURSOR, sizeof(bpl::Ascii::SAVE_CURSOR));
                        for (size_t i = cursorPosition; i < line.size(); i++) outputStream.write(line[i]);
                        outputStream.write(bpl::Ascii::RESTORE_CURSOR, sizeof(bpl::Ascii::RESTORE_CURSOR));
                    }
                }
            }

            switch (byte)
            {
                case bpl::Ascii::CR:
                {
                    editProvider.commit();
                    keepReading = false;
                    break;
                }

                case bpl::Ascii::BS:
                case bpl::Ascii::BS_AS_DEL:
                {
                    if ((cursorPosition > 0) && (line.size() > 0))
                    {
                        cursorPosition--;
                        line.erase(cursorPosition, 1);

                        if (echo)
                        {
                            outputStream.write(bpl::Ascii::BS_SAVE_CURSOR_ERASE_TO_EOL, sizeof(bpl::Ascii::BS_SAVE_CURSOR_ERASE_TO_EOL));
                            for (size_t i = cursorPosition; i < line.size(); i++) outputStream.write(line[i]);
                            outputStream.write(bpl::Ascii::RESTORE_CURSOR, sizeof(bpl::Ascii::RESTORE_CURSOR));
                        }
                    }

                    break;
                }

                default:
                {
                    // note, insert() will append when the cursorPosition == line.size()
                    //
                    line.insert(cursorPosition, 1, char(byte));
                    cursorPosition++;
                }
            }
        }
        else
        {
            asm volatile ("wfi");
        }
    }

    return line;
}

/*
// note, implements CR, BS and DEL handling, consumes the cursor keys
//
const std::pmr::string bpl::TextReader::readln() const
{
    uint8_t byte;
    auto line = std::pmr::string();

    auto handleCursor = false;
    auto keepReading = true;
    while (keepReading)
    {
        if (read(byte))
        {
            // consume the cursor keys
            //
            if ((!handleCursor) && (byte == bpl::Ascii::ESC))
            {
                handleCursor = true;
                continue;
            }

            if (handleCursor)
            {
                if (byte == '[') continue;
                handleCursor = false;

                // check for UP, DOWN, RIGHT and LEFT respectively
                //
                if ((byte == 'A') || (byte == 'B') || (byte == 'C') || (byte == 'D')) continue;
            }

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
                    if (line.size() > 0)
                    {
                        line.pop_back();
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
                    if (line.size() > 0)
                    {
                        line.pop_back();
                        if (echo) outputStream.write(bpl::Ascii::BS_AS_DEL);
                    }

                    break;
                }

                default:
                    line.push_back(byte);
            }
        }
        else
        {
            asm volatile ("wfi");
        }
    }

    return line;
}
*/
