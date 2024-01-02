//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32F4_PWR_H
#define BPL_STM32F4_PWR_H

class Pwr
{
    public:
        inline static constexpr uint32_t CR = 0x00;
        inline static constexpr uint32_t CR_DBP = 1 << 8;
        inline static constexpr uint32_t CR_ODEN = 1 << 16;
        inline static constexpr uint32_t CR_ODSWEN = 1 << 17;

        inline static constexpr uint32_t CSR = 0x04;
        inline static constexpr uint32_t CSR_ODRDY = 1 << 16;
        inline static constexpr uint32_t CSR_ODSWRDY = 1 << 17;
};

#endif
