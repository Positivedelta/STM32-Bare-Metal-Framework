//
// (c) Bit Parallel Ltd, September 2023
//

#include <cstring>

#include "ascii.hpp"
#include "print_writer.hpp"

bpl::PrintWriter::PrintWriter(const bpl::OutputStream& outputStream):
    outputStream(outputStream) {
}

const bool bpl::PrintWriter::print(const uint8_t byte) const
{
    return outputStream.write(byte);
}

const uint8_t bpl::PrintWriter::print(const char* text) const
{
    return outputStream.write((uint8_t*)text, strlen(text));
}

const uint8_t bpl::PrintWriter::print(const std::string_view& text) const
{
    return outputStream.write((uint8_t*)text.data(), text.size());
}

const uint8_t bpl::PrintWriter::println(const char* text) const
{
    const auto size = strlen(text);
    uint8_t bytesWritten = outputStream.write((uint8_t*)text, size);
    if (bytesWritten == size)
    {
        bytesWritten = bytesWritten + outputStream.write(bpl::Ascii::NEW_LINE, sizeof(bpl::Ascii::NEW_LINE));
    }

    return bytesWritten;
}

const uint8_t bpl::PrintWriter::println(const std::string_view& text) const
{
    uint8_t bytesWritten = outputStream.write((uint8_t*)text.data(), text.size());
    if (bytesWritten == text.size())
    {
        bytesWritten = bytesWritten + outputStream.write(bpl::Ascii::NEW_LINE, sizeof(bpl::Ascii::NEW_LINE));
    }

    return bytesWritten;
}

const uint8_t bpl::PrintWriter::println() const
{
    return outputStream.write(bpl::Ascii::NEW_LINE, sizeof(bpl::Ascii::NEW_LINE));
}
