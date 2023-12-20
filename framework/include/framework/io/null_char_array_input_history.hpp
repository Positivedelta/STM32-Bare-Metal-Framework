//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_NULL_CHAR_ARRAY_INPUT_HISTORY_H
#define BPL_NULL_CHAR_ARRAY_INPUT_HISTORY_H

#include <cstddef>

#include "framework/io/input_history.hpp"

namespace bpl
{
    template<size_t N>
    class NullCharArrayInputHistory : public bpl::InputHistory<const char*>
    {
        private:
            char workingBuffer[N];

        public:
            const char*& emptyBuffer() override
            {
                return workingBuffer;
            }

            const char*& buffer() override
            {
                return workingBuffer;
            }

            const bool back() override
            {
                return false;
            }

            const bool forward() override
            {
                return false;
            }

            void commit() override
            {
            }
    };
}

#endif
