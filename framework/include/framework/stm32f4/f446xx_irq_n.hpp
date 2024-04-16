//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_STM32_F446XX_IRQ_H
#define BPL_STM32_F446XX_IRQ_H

#include <cstdint>

class F446IRQn
{
    public:
        inline static constexpr uint32_t Wwdg                = 0;
        inline static constexpr uint32_t Pvd                 = 1;
        inline static constexpr uint32_t TampStamp           = 2;
        inline static constexpr uint32_t RtcWkup             = 3;
        inline static constexpr uint32_t Flash               = 4;
        inline static constexpr uint32_t Rcc                 = 5;
        inline static constexpr uint32_t Exti0               = 6;
        inline static constexpr uint32_t Exti1               = 7;
        inline static constexpr uint32_t Exti2               = 8;
        inline static constexpr uint32_t Exti3               = 9;
        inline static constexpr uint32_t Exti4               = 10;
        inline static constexpr uint32_t Dma1Stream0         = 11;
        inline static constexpr uint32_t Dma1Stream1         = 12;
        inline static constexpr uint32_t Dma1Stream2         = 13;
        inline static constexpr uint32_t Dma1Stream3         = 14;
        inline static constexpr uint32_t Dma1Stream4         = 15;
        inline static constexpr uint32_t Dma1Stream5         = 16;
        inline static constexpr uint32_t Dma1Stream6         = 17;
        inline static constexpr uint32_t Adc                 = 18;
        inline static constexpr uint32_t Can1Tx              = 19;
        inline static constexpr uint32_t Can1Rx0             = 20;
        inline static constexpr uint32_t Can1Rx1             = 21;
        inline static constexpr uint32_t Can1Sce             = 22;
        inline static constexpr uint32_t Exti9To5            = 23;
        inline static constexpr uint32_t Timer1BrkTimer9     = 24;
        inline static constexpr uint32_t Timer1UpTimer10     = 25;
        inline static constexpr uint32_t Timer1TrgComTimer11 = 26;
        inline static constexpr uint32_t Timer1Cc            = 27;
        inline static constexpr uint32_t Timer2              = 28;
        inline static constexpr uint32_t Timer3              = 29;
        inline static constexpr uint32_t Timer4              = 30;
        inline static constexpr uint32_t I2C1Ev              = 31;
        inline static constexpr uint32_t I2C1Er              = 32;
        inline static constexpr uint32_t I2C2Ev              = 33;
        inline static constexpr uint32_t I2C2Er              = 34;
        inline static constexpr uint32_t Spi1                = 35;
        inline static constexpr uint32_t Spi2                = 36;
        inline static constexpr uint32_t Usart1              = 37;
        inline static constexpr uint32_t Usart2              = 38;
        inline static constexpr uint32_t Usart3              = 39;
        inline static constexpr uint32_t Exti15To10          = 40;
        inline static constexpr uint32_t RtcAlarm            = 41;
        inline static constexpr uint32_t OtgFsWkup           = 42;
        inline static constexpr uint32_t Timer8BrkTimer12    = 43;
        inline static constexpr uint32_t Timer8UpTimer13     = 44;
        inline static constexpr uint32_t Timer8TrgComTimer14 = 45;
        inline static constexpr uint32_t Timer8Cc            = 46;
        inline static constexpr uint32_t Dma1Stream7         = 47;
        inline static constexpr uint32_t Fmc                 = 48;
        inline static constexpr uint32_t Sdio                = 49;
        inline static constexpr uint32_t Timer5              = 50;
        inline static constexpr uint32_t Spi3                = 51;
        inline static constexpr uint32_t Uart4               = 52;
        inline static constexpr uint32_t Uart5               = 53;
        inline static constexpr uint32_t Timer6Dac           = 54;
        inline static constexpr uint32_t Timer7              = 55;
        inline static constexpr uint32_t Dma2Stream0         = 56;
        inline static constexpr uint32_t Dma2Stream1         = 57;
        inline static constexpr uint32_t Dma2Stream2         = 58;
        inline static constexpr uint32_t Dma2Stream3         = 59;
        inline static constexpr uint32_t Dma2Stream4         = 60;
        inline static constexpr uint32_t Can2Tx              = 63;
        inline static constexpr uint32_t Can2Rx0             = 64;
        inline static constexpr uint32_t Can2Rx1             = 65;
        inline static constexpr uint32_t Can2Sce             = 66;
        inline static constexpr uint32_t OtgFs               = 67;
        inline static constexpr uint32_t Dma2Stream5         = 68;
        inline static constexpr uint32_t Dma2Stream6         = 69;
        inline static constexpr uint32_t Dma2Stream7         = 70;
        inline static constexpr uint32_t Usart6              = 71;
        inline static constexpr uint32_t I2C3Ev              = 72;
        inline static constexpr uint32_t I2C3Er              = 73;
        inline static constexpr uint32_t OtgHsEp1Out         = 74;
        inline static constexpr uint32_t OtgHsEp1In          = 75;
        inline static constexpr uint32_t OtgHsWkup           = 76;
        inline static constexpr uint32_t OtgHs               = 77;
        inline static constexpr uint32_t Dcmi                = 78;
        inline static constexpr uint32_t Fpu                 = 81;
        inline static constexpr uint32_t Spi4                = 84;
        inline static constexpr uint32_t Sai1                = 87;
        inline static constexpr uint32_t Sai2                = 91;
        inline static constexpr uint32_t QuadSpi             = 92;
        inline static constexpr uint32_t Cec                 = 93;
        inline static constexpr uint32_t SpdifRx             = 94;
        inline static constexpr uint32_t FmpI2C1Ev           = 95;
        inline static constexpr uint32_t FmpI2C1Er           = 96;

        // exception type IRQ numbers
        // notes 1, Nmi and HardFault are NOT supported by the Nvic::setPriority() method
        //       2, they get ignored if used
        //
        inline static constexpr uint32_t Nmi                 = uint32_t(-14);
        inline static constexpr uint32_t HardFault           = uint32_t(-13);
        inline static constexpr uint32_t MemManFault         = uint32_t(-12);
        inline static constexpr uint32_t BusFault            = uint32_t(-11);
        inline static constexpr uint32_t UsageFault          = uint32_t(-10);
        inline static constexpr uint32_t SVCall              = uint32_t(-5);
        inline static constexpr uint32_t PendSV              = uint32_t(-2);
        inline static constexpr uint32_t SysTick             = uint32_t(-1);
};

#endif
