//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_STRING_EDIT_BUFFER_WITH_HISTORY_H
#define BPL_STRING_EDIT_BUFFER_WITH_HISTORY_H

#include <bit>
#include <cstdint>
#include <string>
#include <vector>

#include "framework/io/editproviders/input_edit_provider.hpp"

namespace bpl
{
    class StringEditBufferWithHistory : public bpl::InputEditProvider<std::pmr::string>
    {
        private:
            const size_t maxHistorySize;
            std::pmr::vector<std::pmr::string> history;
            std::pmr::string editBuffer;
            std::pmr::string uncommittedEditBuffer;
            size_t currentSize;
            int32_t commitIndex, index;

        // notes 1, for efficiency the maximum history size must be a power of 2
        //       2, as a precaution, non powers of 2 will be rounded up to the nearest power
        //
        public:
            StringEditBufferWithHistory(const size_t historySize);

            std::pmr::string& emptyBuffer() override;
            std::pmr::string& buffer() override;
            bool back() override;
            bool forward() override;
            void commit() override;
    };
}

#endif
