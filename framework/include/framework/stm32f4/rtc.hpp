//
// (c) Bit Parallel Ltd, January 2024
//

#ifndef BPL_STM32F4_RTC_H
#define BPL_STM32F4_RTC_H

class Rtc
{
    public:
        inline static constexpr uint32_t CR = 0x08;
        inline static constexpr uint32_t CR_WUCKSEL_MASK = 7;
        inline static constexpr uint32_t CR_WUTE = 1 << 10;
        inline static constexpr uint32_t CR_WUTIE = 1 << 14;

        inline static constexpr uint32_t ISR = 0x0c;
        inline static constexpr uint32_t ISR_WUTWF = 1 << 2;
        inline static constexpr uint32_t ISR_WUTF = 1 << 10;

        inline static constexpr uint32_t WUTR = 0x14;
        inline static constexpr uint32_t WPR = 0x24;
};

#endif
