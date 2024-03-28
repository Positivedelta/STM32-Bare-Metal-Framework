//
// (c) Bit Parallel Ltd, December 2023
//

#include "framework/utils/string_utils.hpp"
#include "framework/io/editproviders/string_edit_buffer_with_history.hpp"

bpl::StringEditBufferWithHistory::StringEditBufferWithHistory(const size_t maxHistorySize):
    maxHistorySize(maxHistorySize), history(std::pmr::vector<std::pmr::string>(std::bit_ceil<size_t>(maxHistorySize))),
    currentSize(0), commitIndex(maxHistorySize), index(0) {
}

std::pmr::string& bpl::StringEditBufferWithHistory::emptyBuffer()
{
    editBuffer.clear();

    return editBuffer;
}

std::pmr::string& bpl::StringEditBufferWithHistory::buffer()
{
    return editBuffer;
}

bool bpl::StringEditBufferWithHistory::back()
{
    if ((currentSize > 0) && (index != int32_t(currentSize)))
    {
        if (index == 0) uncommittedEditBuffer = editBuffer;

        const auto i = (commitIndex - index) & (maxHistorySize - 1);
        editBuffer = history[i];
        index++;

        return true;
    }

    return false;
}

bool bpl::StringEditBufferWithHistory::forward()
{
    if ((currentSize > 0) && (index > 0))
    {
        index--;
        if (index == 0)
        {
            editBuffer = uncommittedEditBuffer;
            return true;
        }

        const auto i = (commitIndex - index + 1) & (maxHistorySize - 1);
        editBuffer = history[i];

        return true;
    }

    return false;
}

void bpl::StringEditBufferWithHistory::commit()
{
    // notes 1, copy the contents of the edit buffer as it's being referenced and will have been rendered to the console
    //       2, only commit trimmed commands to the history
    //
    auto command = editBuffer;
    bpl::StringUtils::trim(command);
    if (command.size() > 0)
    {
        commitIndex = (commitIndex + 1) & (maxHistorySize - 1);
        history[commitIndex] = command;

        if (currentSize < maxHistorySize) currentSize++;
    }

    index = 0;
}
