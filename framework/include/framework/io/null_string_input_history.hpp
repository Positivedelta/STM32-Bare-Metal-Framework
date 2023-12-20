//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_NULL_STRING_INPUT_HISTORY_H
#define BPL_NULL_STRING_INPUT_HISTORY_H

#include <string>

#include "framework/io/input_history.hpp"

namespace bpl
{
    class NullStringInputHistory : public bpl::InputHistory<std::pmr::string>
    {
        private:
            std::pmr::string workingBuffer;

        public:
            std::pmr::string& emptyBuffer() override;
            std::pmr::string& buffer() override;
            const bool back() override;
            const bool forward() override;
            void commit() override;
    };
}

#endif
