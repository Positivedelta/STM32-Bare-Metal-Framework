//
// (c) Bit Parallel Ltd, September 2023
//

#include "null_output_stream.hpp"
#include "text_reader.hpp"

bpl::TextReader::TextReader(const InputStream& inputStream):
    inputStream(inputStream), outputStream(NullOutputStream()), echo(false) {
}

bpl::TextReader::TextReader(const bpl::InputStream& inputStream, const bpl::OutputStream& outputStream):
    inputStream(inputStream), outputStream(outputStream), echo(true) {
}

const bool bpl::TextReader::read(uint8_t& byte) const
{
    const bool hasByte = inputStream.read(byte);
    if (hasByte && echo)
    {
        outputStream.write(byte);
        if (byte == bpl::Ascii::CR) outputStream.write(bpl::Ascii::LF);
    }

    return hasByte;
}

const std::pmr::string bpl::TextReader::readln() const
{
    uint8_t byte;
    auto line = std::pmr::string();
    while (true)
    {
        if (read(byte))
        {
            if (byte == bpl::Ascii::CR) break;
            line.push_back(byte);
        }
        else
        {
            asm volatile ("wfi");
        }
    }

    return line;
}
