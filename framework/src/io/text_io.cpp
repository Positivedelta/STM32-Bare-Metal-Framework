//
// (c) Bit Parallel Ltd, October 2023
//

#include "framework/io/text_io.hpp"

bpl::TextIO::TextIO(const bpl::IOStream& ioStream, const bool localEcho):
    textReader((localEcho) ? bpl::TextReader(ioStream.getInputStream(), ioStream.getOutputStream()) : bpl::TextReader(ioStream.getInputStream())),
    printWriter(bpl::PrintWriter(ioStream.getOutputStream())), localEcho(localEcho) {
};

bpl::TextIO::TextIO(const bpl::InputStream& inputStream, const bpl::OutputStream& outputStream, const bool localEcho):
    textReader((localEcho) ? bpl::TextReader(inputStream, outputStream) : bpl::TextReader(inputStream)),
    printWriter(bpl::PrintWriter(outputStream)), localEcho(localEcho) {
};

const bpl::TextReader& bpl::TextIO::getTextReader() const
{
    return textReader;
}

const bpl::PrintWriter& bpl::TextIO::getPrintWriter() const
{
    return printWriter;
}

bool bpl::TextIO::hasLocalEcho() const
{
    return localEcho;
}
