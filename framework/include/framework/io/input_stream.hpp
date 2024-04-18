//
// (c) Bit Parallel Ltd, 2022
//

#ifndef BPL_INPUT_STREAM_H
#define BPL_INPUT_STREAM_H

#include <cstdint>

#include "framework/io/byte_listener.hpp"

namespace bpl
{
    class InputStream
    {
        public:
            virtual bool read(uint8_t& byte) const = 0;
    };
}

#endif
