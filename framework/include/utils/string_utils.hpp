//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_STRING_UTILS_H
#define BPL_STRING_UTILS_H

#include <memory_resource>
#include <string>

namespace bpl
{
    class StringUtils
    {
        public:
            static void trim(std::pmr::string& s);
    };
}

#endif
