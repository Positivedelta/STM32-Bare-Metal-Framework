//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_NULL_OUTPUT_STREAM_H
#define BPL_NULL_OUTPUT_STREAM_H

#include <cstdint>

#include "output_stream.hpp"

namespace bpl
{
    class NullOutputStream : public bpl::OutputStream
    {
        public:
            virtual const bool write(const uint8_t byte) const override;
            virtual const uint32_t write(const uint8_t bytes[], const uint32_t length) const override;
    };
}

#endif
