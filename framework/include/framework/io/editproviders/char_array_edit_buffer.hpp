//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_CHAR_ARRAY_EDIT_BUFFER_H
#define BPL_CHAR_ARRAY_EDIT_BUFFER_H

#include <cstddef>

#include "framework/io/editproviders/input_edit_provider.hpp"

namespace bpl
{
    template<size_t maxSize>
    class CharArrayEditBuffer : public bpl::InputEditProvider<char*>
    {
        private:
            char workingBuffer[maxSize];
            char* workingBufferPtr = workingBuffer;

        public:
            char*& emptyBuffer() override
            {
                workingBuffer[0] = '\0';

                return workingBufferPtr;
            }

            char*& buffer() override
            {
                return workingBufferPtr;
            }

            bool back() override
            {
                return false;
            }

            bool forward() override
            {
                return false;
            }

            void commit() override
            {
            }
    };
}

#endif
