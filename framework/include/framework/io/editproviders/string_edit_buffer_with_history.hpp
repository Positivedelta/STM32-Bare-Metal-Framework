//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_STRING_EDIT_BUFFER_WITH_HISTORY_H
#define BPL_STRING_EDIT_BUFFER_WITH_HISTORY_H

#include <cstdint>
#include <deque>
#include <string>

#include "framework/io/editproviders/input_edit_provider.hpp"

namespace bpl
{
    class StringEditBufferWithHistory : public bpl::InputEditProvider<std::pmr::string>
    {
        private:
            const size_t maxHistorySize;
            int32_t index;
            std::deque<std::pmr::string> history;
            std::pmr::string workingBuffer;
            std::pmr::string uncommittedBuffer;

        public:
            StringEditBufferWithHistory(const size_t historySize);

            std::pmr::string& emptyBuffer() override;
            std::pmr::string& buffer() override;
            const bool back() override;
            const bool forward() override;
            void commit() override;
    };
}

#endif
