//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_TEXT_IO_H
#define BPL_TEXT_IO_H

#include "framework/io/io_stream.hpp"
#include "framework/io/text_reader.hpp"
#include "framework/io/print_Writer.hpp"

namespace bpl
{
    class TextIO
    {
        private:
            const bpl::TextReader textReader;
            const bpl::PrintWriter printWriter;
            const bool localEcho;

        public:
            TextIO(const bpl::IOStream& ioStream, const bool localEcho = true);
            TextIO(const bpl::InputStream& inputStream, const bpl::OutputStream& outputStream, const bool localEcho = true);
            const bpl::TextReader& getTextReader() const;
            const bpl::PrintWriter& getPrintWriter() const;
            bool hasLocalEcho() const;
    };
}

#endif
