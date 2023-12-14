//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_STRING_UTILS_H
#define BPL_STRING_UTILS_H

#include <cstdint>
#include <memory_resource>
#include <string>
#include <vector>

namespace bpl
{
    class StringUtils
    {
        public:
            static void trim(std::pmr::string& string);
            static void strip(std::pmr::string& string, const unsigned char c);
            static void tokenize(const std::string_view& stringView, const char separator, std::pmr::vector<std::string_view>& tokens);

            // added as std::stoi() uses std::string and throws exceptions
            //
            static bool stoi(const std::string_view& stringView, int32_t& value);
    };
}

#endif
