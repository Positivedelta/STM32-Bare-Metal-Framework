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
            inline static const uint8_t CR = 0x0d;
            inline static const uint8_t LF = 0x0a;
            inline static const uint8_t SPACE = 0x20;
            inline static const uint8_t ESC = 0x1b;

            // notes 1, Putty can map the BS key to either 0x08 or 0x7f
            //       2, applications should probably check for both options
            //       3, by default, the DEL key transmits ^[3~
            //
            inline static const uint8_t BS = 0x08;
            inline static const uint8_t BS_AS_DEL = 0x7f;
            inline static const uint8_t DEL_KEY[] = {ESC, '[', '3', '~'};

            inline static const uint8_t NEW_LINE[] = {CR, LF};

            //
            // composite VT100 escape codes
            //

            inline static const uint8_t CURSOR_LEFT[] = {ESC, '[', 'D'};
            inline static const uint8_t CURSOR_RIGHT[] = {ESC, '[', 'C'};

            inline static const uint8_t SAVE_CURSOR[] = {ESC, '7'};
            inline static const uint8_t RESTORE_CURSOR[] = {ESC, '8'};

            // note 1, ERASE_CURSOR_TO_EOL can also be implemented as ^[0K
            //      2, using the shorter / optimised version
            //
            inline static const uint8_t ERASE_CURSOR_TO_EOL[] = {ESC, '[', 'K'};
            inline static const uint8_t ERASE_SOL_TO_CURSOR[] = {ESC, '[', '1', 'K'};
            inline static const uint8_t ERASE_LINE[] = {ESC, '[', '2', 'K', '\r'};

            // composite escape codes
            //
            inline static const uint8_t BS_SAVE_CURSOR_ERASE_TO_EOL[] = {BS, ESC, '7', ESC, '[', 'K'};
    };
}

#endif
