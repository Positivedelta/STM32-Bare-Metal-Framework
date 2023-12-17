//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_ASCII_H
#define BPL_ASCII_H

#include <cstdint>

namespace bpl
{
    class Ascii
    {
        public:
            inline static const uint8_t BS = 0x08;
            inline static const uint8_t CR = 0x0d;
            inline static const uint8_t LF = 0x0a;
            inline static const uint8_t SPACE = 0x20;
            inline static const uint8_t DEL = 0x7f;
            inline static const uint8_t ESC = 0x1b;

            inline static const uint8_t NEW_LINE[] = {CR, LF};

            //
            // composite VT100 escape codes
            //

            inline static const uint8_t CURSOR_LEFT[] = {ESC, '[', 'D'};
            inline static const uint8_t CURSOR_RIGHT[] = {ESC, '[', 'C'};

            // note 1, CLEAR_FROM_CURSOR_RIGHT can also be implemented as ^[[0K
            //      2, using the shorter / optimised version
            //
            inline static const uint8_t CLEAR_FROM_CURSOR_RIGHT[] = {ESC, '[', '[', 'K'};
            inline static const uint8_t CLEAR_FROM_CURSOR_LEFT[] = {ESC, '[', '[', '1', 'K'};
            inline static const uint8_t CLEAR_LINE[] = {ESC, '[', '[', '2', 'K'};
    };
}

#endif
