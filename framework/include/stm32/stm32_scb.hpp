//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32SCB_H
#define BPL_STM32SCB_H

class Scb
{
    public:
        inline static constexpr uint32_t AIRCR = 0x0c;
        inline static constexpr uint32_t SHPR1 = 0x18;
        inline static constexpr uint32_t SHPR2 = 0x1c;
        inline static constexpr uint32_t SHPR3 = 0x20;
};

#endif
