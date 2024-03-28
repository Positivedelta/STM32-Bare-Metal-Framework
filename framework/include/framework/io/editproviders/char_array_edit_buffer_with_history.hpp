//
// (c) Bit Parallel Ltd, January 2024
//

#ifndef BPL_CHAR_ARRAY_EDIT_BUFFER_WITH_HISTORY_H
#define BPL_CHAR_ARRAY_EDIT_BUFFER_WITH_HISTORY_H

#include <array>
#include <cstring>

#include "framework/io/editproviders/input_edit_provider.hpp"
#include "framework/utils/string_utils.hpp"

namespace bpl
{
    template<size_t maxLineSize, size_t maxHistorySize>
    class CharArrayEditBufferWithHistory : public bpl::InputEditProvider<char*>
    {
        private:
            std::array<std::array<char, maxLineSize>, maxHistorySize> history;
            std::array<char, maxLineSize> editBuffer;
            std::array<char, maxLineSize> uncommittedEditBuffer{};
            char* editBufferPtr;
            char* uncommittedEditBufferPtr;
            size_t currentSize;
            int32_t commitIndex, index;

        public:
            CharArrayEditBufferWithHistory():
                history(std::array<std::array<char, maxLineSize>, maxHistorySize>{}),
                editBuffer(std::array<char, maxLineSize>{}), uncommittedEditBuffer(std::array<char, maxLineSize>{}),
                editBufferPtr(editBuffer.data()), uncommittedEditBufferPtr(uncommittedEditBuffer.data()),
                currentSize(0), commitIndex(8), index(0) {
            }

            char*& emptyBuffer() override
            {
                editBuffer[0] = '\0';

                return editBufferPtr;
            }

            char*& buffer() override
            {
                return editBufferPtr;
            }

            bool back() override
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

            bool forward() override
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

            void commit() override
            {
                // notes 1, operate on a copy the contents of the edit buffer as it's being referenced and will have been rendered to the console
                //       2, only commit trimmed commands to the history
                //
                auto command = bpl::StringUtils::trim<maxLineSize>(editBuffer);
                if (std::strlen(command.data()) > 0)
                {
                    commitIndex = (commitIndex + 1) & (maxHistorySize - 1);
                    history[commitIndex] = command;

                    if (currentSize < maxHistorySize) currentSize++;
                }

                index = 0;
            }
    };
}

#endif
