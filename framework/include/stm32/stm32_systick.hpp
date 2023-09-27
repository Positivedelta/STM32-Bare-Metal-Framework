//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32STK_H
#define BPL_STM32STK_H

class Stk
{
    public:
        inline static constexpr uint32_t CTRL = 0x00;
        inline static constexpr uint32_t LOAD = 0x04;
        inline static constexpr uint32_t VAL = 0x08;
        inline static constexpr uint32_t CALIB = 0x0c;
};

#endif
