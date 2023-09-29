//
// (c) Bit Parallel Ltd, September 2023
//

#include "null_output_stream.hpp"

const bool bpl::NullOutputStream::write(const uint8_t byte) const
{
    return true;
}

const uint32_t bpl::NullOutputStream::write(const uint8_t bytes[], const uint32_t length) const
{
    return length;
}
