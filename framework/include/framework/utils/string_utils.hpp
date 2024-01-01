//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_STRING_UTILS_H
#define BPL_STRING_UTILS_H

#include <array>
#include <cstdint>
#include <cstring>
#include <memory_resource>
#include <string>
#include <vector>

namespace bpl
{
    class StringUtils
    {
        public:
            static void trim(std::pmr::string& string);

            template<size_t maxSize>
            static std::array<char, maxSize> trim(std::array<char, maxSize>& string)
            {
                auto character = string.front();
                if (character == '\0') return string;

                // for a maximum length string the final character is expected to be a '\0', add one anyway just to make the trim() method safe
                //
                string.back() = '\0';

                int32_t start = 0;
                int32_t end = std::strlen(string.data());
                while ((character == ' ') && (character != '\0')) character = string[++start];
                while (string[--end] == ' ');

                int32_t size = 0;
                std::array<char, maxSize> trimmed;
                for (int32_t in = start; in <= end; in++) trimmed[size++] = string[in];
                trimmed[size] = '\0';

                return trimmed;
            }

            static void strip(std::pmr::string& string, const unsigned char c);
            static void tokenize(const std::string_view& stringView, const char separator, std::pmr::vector<std::string_view>& tokens);

            // added as std::stoi() uses std::string and throws exceptions
            //
            static bool stoi(const std::string_view& stringView, int32_t& value);
    };
}

#endif
