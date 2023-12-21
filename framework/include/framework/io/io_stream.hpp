//
// (c) Bit Parallel Ltd, 2022
//

#ifndef BPL_IO_STREAM_H
#define BPL_IO_STREAM_H

#include <framework/io/input_stream.hpp>
#include <framework/io/output_stream.hpp>

namespace bpl
{
    class IOStream
    {
        public:
            virtual const bpl::InputStream& getInputStream() const = 0;
            virtual const bpl::OutputStream& getOutputStream() const = 0;
    };
}

#endif
