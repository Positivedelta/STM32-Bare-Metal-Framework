//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32USART_H
#define BPL_STM32USART_H

class Usart
{
    public:
        inline static constexpr uint32_t SR = 0x00;
        inline static constexpr uint32_t SR_RXNE = 1 << 5;
        inline static constexpr uint32_t SR_TXE = 1 << 7;

        inline static constexpr uint32_t DR = 0x04;
        inline static constexpr uint32_t BRR = 0x08;

        inline static constexpr uint32_t CR1 = 0x0c;
        inline static constexpr uint32_t CR1_RE = 1 << 2;
        inline static constexpr uint32_t CR1_TE = 1 << 3;
        inline static constexpr uint32_t CR1_RXNEIE = 1 << 5;
        inline static constexpr uint32_t CR1_TXEIE = 1 << 7;
        inline static constexpr uint32_t CR1_UE = 1 << 13;

        inline static constexpr uint32_t CR2 = 0x10;
        inline static constexpr uint32_t CR3 = 0x14;
        inline static constexpr uint32_t GTPR = 0x18;
};

#endif
