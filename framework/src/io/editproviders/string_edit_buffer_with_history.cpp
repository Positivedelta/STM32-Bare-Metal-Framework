//
// (c) Bit Parallel Ltd, December 2023
//

#include "framework/utils/string_utils.hpp"
#include "framework/io/editproviders/string_edit_buffer_with_history.hpp"

bpl::StringEditBufferWithHistory::StringEditBufferWithHistory(const size_t maxHistorySize):
    maxHistorySize(maxHistorySize), index(-1) {
}

std::pmr::string& bpl::StringEditBufferWithHistory::emptyBuffer()
{
    workingBuffer.clear();

    return workingBuffer;
}

std::pmr::string& bpl::StringEditBufferWithHistory::buffer()
{
    return workingBuffer;
}

const bool bpl::StringEditBufferWithHistory::back()
{
    if (index == -1) uncommittedBuffer = workingBuffer;

    const auto maxIndex = int32_t(history.size()) - 1;
    if ((maxIndex >= 0) && (index < maxIndex))
    {
        workingBuffer = history[++index];
        return true;
    }

    return false;
}

const bool bpl::StringEditBufferWithHistory::forward()
{
    if ((history.size() > 0) && (index >= 0))
    {
        index--;
        if (index == -1)
        {
            workingBuffer = uncommittedBuffer;
            return true;
        }

        workingBuffer = history[index];
        return true;
    }

    return false;
}

void bpl::StringEditBufferWithHistory::commit()
{
    // notes 1, don't alter the existing raw buffer as it is being used externally
    //       2, only commit trimmed commands to the history
    //
    auto command = workingBuffer;
    bpl::StringUtils::trim(command);
    if (command.size() > 0)
    {
        if (history.size() == maxHistorySize) history.pop_back();
        history.push_front(command);
    }

    index = -1;
    uncommittedBuffer = std::pmr::string();
}
