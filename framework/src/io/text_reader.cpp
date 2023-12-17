//
// (c) Bit Parallel Ltd, September 2023
//

#include "framework/io/null_output_stream.hpp"
#include "framework/io/text_reader.hpp"

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

// note, implements CR, BS and DEL handling
//
const std::pmr::string bpl::TextReader::readln() const
{
    uint8_t byte;
    auto line = std::pmr::string();
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

                case bpl::Ascii::DEL:
                {
                    if (line.size() > 0)
                    {
                        line.pop_back();
                        if (echo) outputStream.write(bpl::Ascii::DEL);
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
