//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_CCITT_CRC_16_H
#define BPL_CCITT_CRC_16_H

#include <array>
#include <cstdint>

#include "framework/utils/data_utils.hpp"

namespace bpl
{
    // supports the various CCITT-16 varients using the following configurations
    //
    // ccitt, poly: 0x1021, seed: 0xffff, reverse: false
    // xmodem, poly: 0x1021, seed: 0, reverse: false
    // kermit, poly: 0x1021, seed: 0, reverse: true
    // ibm, ploy: 0x8005, seed: 0, reverse: true
    //
    // notes 1, if the data is already byte reversed, negate the above boolean reverse flags
    //       2 when the reverse flag is set, the polynomial is reversed internally
    //         i.e. do not swap 0x1021 for 0x8408 etc. if reverse is specified
    //
    template<int32_t size, uint16_t polynomial, bool reverse = false>
    class CcittCrc16
    {
        private:
            static constexpr uint16_t bitLength = 16;
            static constexpr uint16_t mask = (1 << (bitLength - 1));
            static constexpr int32_t lookupEntries = 256;

            // note, a CRC table is essentially the CRCs of the one-byte messages from 0 to 255
            //
            static constexpr std::array<uint16_t, lookupEntries> generateTable()
            {
                auto table = std::array<uint16_t, lookupEntries>();
                for (auto i = 0; i < lookupEntries; i++)
                {
                    uint16_t crc = i << 8;
                    for (auto bit = 0; bit < 8; bit++)
                    {
                        if (crc & mask)
                        {
                            crc = (crc << 1) ^ polynomial;
                        }
                        else
                        {
                            crc <<= 1;
                        }
                    }

                    if (reverse)
                    {
                        table[bpl::DataUtils::reverse8(i)] = bpl::DataUtils::reverse16(crc);
                    }
                    else
                    {
                        table[i] = crc;
                    }
                }

                return table;
            }

            static constexpr std::array<uint16_t, lookupEntries> crcTable = generateTable();

        public:
            uint16_t compute(const std::array<uint8_t, size>& data, const uint16_t initValue = 0) const
            {
                uint16_t crc = initValue;
                for (uint32_t i = 0; i < size; i++)
                {
                    if constexpr (reverse)
                    {
                        const uint8_t index = crc ^ data[i];
                        crc = crcTable[index] ^ (crc >> 8);
                    }
                    else
                    {
                        const uint8_t index = (crc >> 8) ^ data[i];
                        crc = crcTable[index] ^ (crc << 8);
                    }
                }

                return crc;
            }
    };
}

#endif
