//
// (c) Bit Parallel Ltd, 2022
//

#ifndef BPL_INPUT_STREAM_H
#define BPL_INPUT_STREAM_H

#include <cstdint>

#include "byte_listener.hpp"

namespace bpl
{
    class InputStream
    {
        public:
            virtual const bool read(uint8_t& byte) const = 0;

            // note, any registered listener must be quick as it will execute in the context of the IRQ handler
            //
            virtual void setByteListener(const bpl::ByteListener& byteListener) const = 0;
    };
}

#endif
