//
// (c) Bit Parallel Ltd, 2022
//

#ifndef BPL_OUTPUT_STREAM_H
#define BPL_OUTPUT_STREAM_H

#include <cstdint>

namespace bpl
{
    class OutputStream
    {
        public:
            virtual const bool write(const uint8_t byte) const = 0;
            virtual const uint32_t write(const uint8_t bytes[], const uint32_t length) const = 0;
    };
}

#endif
