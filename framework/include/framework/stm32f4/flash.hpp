//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32F4_FLASH_H
#define BPL_STM32F4_FLASH_H

class Flash
{
    public:
        inline static constexpr uint32_t ACR = 0x00;
        inline static constexpr uint32_t ACR_LATENCY_MASK = 0b1111;
        inline static constexpr uint32_t ACR_PRFTEN = 1 << 8;
        inline static constexpr uint32_t ACR_ICEN = 1 << 9;
        inline static constexpr uint32_t ACR_DCEN = 1 << 10;
        inline static constexpr uint32_t ACR_ICRST = 1 << 11;
        inline static constexpr uint32_t ACR_DCRST = 1 << 12;
};

#endif
