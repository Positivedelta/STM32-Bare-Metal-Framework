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
            // FIXME! needs to be used carefully as there is no real bounds checking on the generated character buffer
            //
            template<int32_t size> requires ((size > 0) && (size <= 256))
            static bool itoc(const int32_t value, int32_t padding, char (&buffer)[size])
            {
                auto index = 1;
                if (value == 0)
                {
                    buffer[0] = '0';
                }
                else
                {
                    index = ftocInsert(value, buffer);
                }

                while (index < padding) buffer[index++] = ' ';
                ftocReverse(buffer, index);
                buffer[index] = 0;
                return true;
            }

            // added as a substitute for std::snprintf() to format floats and avoid the chance of dynamic memory being allocated
            // FIXME! needs to be used carefully as there is no real bounds checking on the generated character buffer
            //
            template<int32_t size> requires ((size > 0) && (size <= 256))
            static bool ftoc(const float value, int32_t decimalPlaces, int32_t padding, char (&buffer)[size])
            {
                const auto factor = int32_t(std::pow(10, decimalPlaces));
                const auto expansion = int32_t(value * factor);

                auto index = 0;
                if (expansion == 0)
                {
                    buffer[index] = '0';

                    // check the buffer size, i.e. the leading '0.' the decimal place '0's and the terminating '\0'
                    // note, that size > 0, as enforced by the template condition
                    //
                    if (size < (3 + decimalPlaces)) return false;

                    buffer[++index] = '.';
                    while (++index < (2 + decimalPlaces)) buffer[index] = '0';
                    buffer[index] = 0;
                }
                else
                {
                    auto right = expansion % factor;
                    if (right == 0)
                    {
                        while (index < decimalPlaces) buffer[index++] = '0';
                    }
                    else
                    {
                        index = ftocInsert(right, buffer);
                        while (index < decimalPlaces) buffer[index++] = '0';
                    }

                    buffer[index++] = '.';

                    auto left = int32_t(value);
                    if (left == 0)
                    {
                        buffer[index++] = '0';
                    }
                    else
                    {
                        index += ftocInsert(left, &buffer[index]);
                    }

                    while (index < padding) buffer[index++] = ' ';
                    ftocReverse(buffer, index);
                    buffer[index] = 0;
                }

                return true;
            }

        private:
            static int32_t ftocInsert(int32_t value, char* buffer)
            {
                auto index = 0;
                while(value > 0)
                {
                    buffer[index++] = '0' + (value % 10);
                    value /= 10;
                }

                return index;
            }

            static void ftocReverse(char *buffer, const int32_t size)
            {
                auto i =0;
                auto j = size - 1;
                while (i < j)
                {
                    const char swap = buffer[i];
                    buffer[i++] = buffer[j];
                    buffer[j--] = swap;
                }
            }
    };
}

#endif
