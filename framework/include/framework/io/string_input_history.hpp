//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_STRING_INPUT_HISTORY_H
#define BPL_STRING_INPUT_HISTORY_H

#include <cstdint>
#include <deque>
#include <string>

#include "framework/io/input_history.hpp"

namespace bpl
{
    class StringInputHistory : public bpl::InputHistory<std::pmr::string>
    {
        private:
            const size_t maxHistorySize;
            int32_t index;
            std::deque<std::pmr::string> history;
            std::pmr::string workingBuffer;
            std::pmr::string uncommittedBuffer;

        public:
            StringInputHistory(const size_t historySize);

            std::pmr::string& emptyBuffer() override;
            std::pmr::string& buffer() override;
            const bool back() override;
            const bool forward() override;
            void commit() override;
    };
}

#endif
