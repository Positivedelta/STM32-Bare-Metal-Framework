//
// (c) Bit Parallel Ltd, December 2023
//

#include "framework/io/null_string_input_history.hpp"

std::pmr::string& bpl::NullStringInputHistory::emptyBuffer()
{
    workingBuffer.clear();

    return workingBuffer;
}

std::pmr::string& bpl::NullStringInputHistory::buffer()
{
    return workingBuffer;
}

const bool bpl::NullStringInputHistory::back()
{
    return false;
}

const bool bpl::NullStringInputHistory::forward()
{
    return false;
}

void bpl::NullStringInputHistory::commit()
{
}
