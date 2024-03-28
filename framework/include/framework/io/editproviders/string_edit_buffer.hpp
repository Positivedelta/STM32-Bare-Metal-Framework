//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_STRING_EDIT_BUFFER_H
#define BPL_STRING_EDIT_BUFFER_H

#include <string>

#include "framework/io/editproviders/input_edit_provider.hpp"

namespace bpl
{
    class StringEditBuffer : public bpl::InputEditProvider<std::pmr::string>
    {
        private:
            std::pmr::string workingBuffer;

        public:
            std::pmr::string& emptyBuffer() override;
            std::pmr::string& buffer() override;
            bool back() override;
            bool forward() override;
            void commit() override;
    };
}

#endif
