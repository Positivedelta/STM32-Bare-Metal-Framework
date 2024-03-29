//
// (c) Bit Parallel Ltd, December 2023
//

#include "framework/io/editproviders/string_edit_buffer.hpp"

std::pmr::string& bpl::StringEditBuffer::emptyBuffer()
{
    workingBuffer.clear();

    return workingBuffer;
}

std::pmr::string& bpl::StringEditBuffer::buffer()
{
    return workingBuffer;
}

bool bpl::StringEditBuffer::back()
{
    return false;
}

bool bpl::StringEditBuffer::forward()
{
    return false;
}

void bpl::StringEditBuffer::commit()
{
}
