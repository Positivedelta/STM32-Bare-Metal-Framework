//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32F4_RCC_H
#define BPL_STM32F4_RCC_H

class Rcc
{
    public:
        inline static constexpr uint32_t CR = 0x00;
        inline static constexpr uint32_t CR_HSEON = 1 << 16;
        inline static constexpr uint32_t CR_HSERDY = 1 << 17;
        inline static constexpr uint32_t CR_PLLON = 1 << 24;
        inline static constexpr uint32_t CR_PLLRDY = 1 << 25;
        inline static constexpr uint32_t CR_PLLSAION = 1 << 28;
        inline static constexpr uint32_t CR_PLLSAIRDY = 1 << 29;

        inline static constexpr uint32_t PLLCFGR = 0x04;
        inline static constexpr uint32_t PLLCFGR_PLLSRC = 1 << 22;
        inline static constexpr uint32_t PLLCFGR_PLLN_SHIFT = 6;
        inline static constexpr uint32_t PLLCFGR_PLLQ_SHIFT = 24;
        inline static constexpr uint32_t PLLCFGR_PLLR_SHIFT = 28;

        inline static constexpr uint32_t CFGR = 0x08;
        inline static constexpr uint32_t CFGR_SWS_SHIFT = 2;
        inline static constexpr uint32_t CFGR_PPRE1_SHIFT = 10;
        inline static constexpr uint32_t CFGR_PPRE2_SHIFT = 13;

        inline static constexpr uint32_t CSR = 0x74;
        inline static constexpr uint32_t CSR_LSION = 1;
        inline static constexpr uint32_t CSR_LSIRDY = 2;

        inline static constexpr uint32_t BDCR = 0x70;
        inline static constexpr uint32_t BDCR_RTCSEL_0 = 1 << 8;
        inline static constexpr uint32_t BDCR_RTCSEL_1 = 1 << 9;
        inline static constexpr uint32_t BDCR_RTCEN = 1 << 15;
        inline static constexpr uint32_t BDCR_BDRST = 1 << 16;

        inline static constexpr uint32_t PLLSAICFGR = 0x88;
        inline static constexpr uint32_t PLLSAICFGR_PLLSAIN_SHIFT = 6;
        inline static constexpr uint32_t PLLSAICFGR_PLLSAIP_SHIFT = 16;
        inline static constexpr uint32_t PLLSAICFGR_PLLSAIQ_SHIFT = 24;

        inline static constexpr uint32_t DCKCFGR2 = 0x94;
        inline static constexpr uint32_t DCKCFGR2_CK48MSEL = 1 << 27;

        inline static constexpr uint32_t APB1RSTR = 0x20;
        inline static constexpr uint32_t APB1RSTR_TIM3RST = 1 << 1;
        inline static constexpr uint32_t APB1RSTR_TIM4RST = 1 << 2;

        inline static constexpr uint32_t AHB1ENR = 0x30;
        inline static constexpr uint32_t AHB1ENR_GPIOA = 0x01;
        inline static constexpr uint32_t AHB1ENR_GPIOB = 0x02;
        inline static constexpr uint32_t AHB1ENR_GPIOC = 0x04;

        inline static constexpr uint32_t APB1ENR = 0x40;
        inline static constexpr uint32_t APB1ENR_TIM3EN = 1 << 1;
        inline static constexpr uint32_t APB1ENR_TIM4EN = 1 << 2;
        inline static constexpr uint32_t APB1ENR_USART2EN = 1 << 17;
        inline static constexpr uint32_t APB1ENR_PWREN = 1 << 28;

        inline static constexpr uint32_t APB2ENR = 0x44;
        inline static constexpr uint32_t APB2ENR_USART1EN = 1 << 4;
        inline static constexpr uint32_t APB2ENR_USART6EN = 1 << 5;
};

#endif
