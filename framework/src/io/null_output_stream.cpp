//
// (c) Bit Parallel Ltd, September 2023
//

#include "framework/io/null_output_stream.hpp"

bool bpl::NullOutputStream::write([[maybe_unused]] const uint8_t byte) const
{
    return true;
}

uint32_t bpl::NullOutputStream::write([[maybe_unused]] const uint8_t bytes[], const uint32_t length) const
{
    return length;
}
