//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32F4_STK_H
#define BPL_STM32F4_STK_H

class Stk
{
    public:
        inline static constexpr uint32_t CTRL = 0x00;
        inline static constexpr uint32_t CTRL_ENABLE = 1;
        inline static constexpr uint32_t CTRL_TICKINT = 1 << 1;
        inline static constexpr uint32_t CTRL_CLKSOURCE = 1 << 2;
        inline static constexpr uint32_t CTRL_COUNT = 1 << 16;

        inline static constexpr uint32_t LOAD = 0x04;
        inline static constexpr uint32_t VAL = 0x08;
        inline static constexpr uint32_t CALIB = 0x0c;
};

#endif
