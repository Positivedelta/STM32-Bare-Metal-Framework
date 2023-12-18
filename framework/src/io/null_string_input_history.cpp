//
// (c) Bit Parallel Ltd, December 2023
//

#include "framework/io/null_string_input_history.hpp"

const bool bpl::NullStringInputHistory::back()
{
    return false;
}

const bool bpl::NullStringInputHistory::forward()
{
    return false;
}

std::pmr::string& bpl::NullStringInputHistory::input()
{
    return data;
}
