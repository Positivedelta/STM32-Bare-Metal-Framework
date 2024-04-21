//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_DATA_UTILS_H
#define BPL_DATA_UTILS_H

#include <array>
#include <cstdint>

//
// notes 1, don't split the non templated constexpr methods between .hpp & .cpp files
//       2, constexpr methods / functions must be fully defined everywhere they are included in order to be available at compile time
//

namespace bpl
{
    class DataUtils
    {
        public:
            static constexpr uint8_t reverse8(const uint8_t byte)
            {
                const uint8_t lookup[16] = {
                    0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e,
                    0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f
                };

                return lookup[byte & 0x0f] << 4 | lookup[byte >> 4];
            }

            static constexpr uint16_t reverse16(const uint16_t word)
            {
                return reverse8(word & 0xff) << 8 | reverse8(word >> 8);
            }

            template<int32_t size>
            static constexpr std::array<uint8_t, size> bitReverseBytes(const std::array<uint8_t, size>& packet)
            {
                auto reversed = std::array<uint8_t, size>();
                for (auto i = 0; i < size; i++) reversed[i] = reverse8(packet[i]);

                return reversed;
            }
    };
}

#endif
