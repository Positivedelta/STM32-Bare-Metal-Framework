//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_ASCII_H
#define BPL_ASCII_H

#include <cstdint>

namespace bpl
{
    class Ascii
    {
        public:
            inline static const uint8_t CR = 0x0d;
            inline static const uint8_t LF = 0x0a;
            inline static const uint8_t NEW_LINE[] = {CR, LF};
    };
}

#endif
