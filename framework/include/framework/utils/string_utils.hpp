//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_STRING_UTILS_H
#define BPL_STRING_UTILS_H

#include <array>
#include <cstdint>
#include <cstring>
#include <cmath>
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

            // added as a substitute for std::snprintf() to format integers and avoid the chance of dynamic memory being allocated
            // FIXME! needs to be used carefully as there is no real bounds checking on the internal buffer manipulation
            //
            template<int32_t size, int32_t padding = 0> requires ((size > 0) && (size <= 256) && (padding >= 0))
            static char* itoc(int32_t value, char (&buffer)[size])
            {
                auto index = size;
                buffer[--index] = 0;

                if (value == 0)
                {
                    buffer[--index] = '0';
                }
                else
                {
                    const auto negative = (value < 0);
                    if (negative) value = -value;

                    while (value > 0)
                    {
                        buffer[--index] = '0' + (value % 10);
                        value /= 10;
                    }

                    if (negative) buffer[--index] = '-';
                }

                while (index >= (size - padding)) buffer[--index] = ' ';
                return &buffer[index];
            }

            // added as a substitute for std::snprintf() to format floats and avoid the chance of dynamic memory being allocated
            // FIXME! needs to be used carefully as there is no real bounds checking on the internal buffer manipulation
            //
            template<int32_t size, int32_t decimalPlaces = 2, int32_t padding = 0> requires ((size > 0) && (size <= 256) && (decimalPlaces >= 0) && (padding >= 0))
            static char* ftoc(float value, char (&buffer)[size])
            {
                auto index = size;
                buffer[--index] = 0;

                constexpr auto factor = uint32_t(std::pow(10, decimalPlaces));
                auto expansion = int32_t(value * factor);
                if (expansion == 0)
                {
                    if constexpr (decimalPlaces > 0)
                    {
                        while (index > (size - 1 - decimalPlaces)) buffer[--index] = '0';
                        buffer[--index] = '.';
                    }

                    buffer[--index] = '0';
                }
                else
                {
                    const auto negative = (value < 0);
                    if (negative)
                    {
                        expansion = -expansion;
                        value = -value;
                    }

                    if constexpr (decimalPlaces > 0)
                    {
                        auto right = expansion % factor;
                        if (right == 0)
                        {
                            while (index > (size - 1 - decimalPlaces)) buffer[--index] = '0';
                        }
                        else
                        {
                            while (right > 0)
                            {
                                buffer[--index] = '0' + (right % 10);
                                right /= 10;
                            }

                            while (index > (size - 1 - decimalPlaces)) buffer[--index] = '0';
                        }

                        buffer[--index] = '.';
                    }

                    auto left = int32_t(value);
                    if (left == 0)
                    {
                        buffer[--index] = '0';
                    }
                    else
                    {
                        while (left > 0)
                        {
                            buffer[--index] = '0' + (left % 10);
                            left /= 10;
                        }
                    }

                    if (negative) buffer[--index] = '-';
                }

                while (index >= (size - padding)) buffer[--index] = ' ';
                return &buffer[index];
            }
    };
}

#endif
