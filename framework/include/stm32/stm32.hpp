//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32_H
#define BPL_STM32_H

#include <cstdint>

#include "stm32_scb.hpp"

class Stm32
{
    public:
        inline static constexpr uint32_t AHB_CLOCK = 180000000;

    private:
        inline static constexpr uint32_t CORE_PERIPHERAL_BASE = 0xe0000000;
        inline static constexpr uint32_t SYSTICK = 0xe010;
        inline static constexpr uint32_t FPU = 0xed88;

        inline static constexpr uint32_t SCS_BASE = 0xe000e000;
        inline static constexpr uint32_t NVIC = 0x0100;
        inline static constexpr uint32_t SCB = 0x0d00;

        inline static constexpr uint32_t AHB1_BASE = 0x40020000;
        inline static constexpr uint32_t RCC = 0x3800;
        inline static constexpr uint32_t GPIO_A = 0x0000;
        inline static constexpr uint32_t GPIO_B = 0x0400;
        inline static constexpr uint32_t GPIO_C = 0x0800;
        inline static constexpr uint32_t FLASH = 0x3c00;

        inline static constexpr uint32_t APB1_BASE = 0x40000000;
        inline static constexpr uint32_t USART_2 = 0x4400;
        inline static constexpr uint32_t TIMER_4 = 0x0800;
        inline static constexpr uint32_t PWR = 0x7000;

    public:
        volatile constexpr static uint32_t& fpu(const uint32_t reg)
        {
            return peripheral(CORE_PERIPHERAL_BASE + FPU + reg);
        }

        volatile constexpr static uint32_t& sysTick(const uint32_t reg)
        {
            return peripheral(CORE_PERIPHERAL_BASE + SYSTICK + reg);
        }

        volatile constexpr static uint32_t& rcc(const uint32_t reg)
        {
            return peripheral(AHB1_BASE + RCC + reg);
        }

        volatile constexpr static uint32_t& gpioA(const uint32_t reg)
        {
            return peripheral(AHB1_BASE + GPIO_A + reg);
        }

        volatile constexpr static uint32_t& gpioB(const uint32_t reg)
        {
            return peripheral(AHB1_BASE + GPIO_B + reg);
        }

        volatile constexpr static uint32_t& gpioC(const uint32_t reg)
        {
            return peripheral(AHB1_BASE + GPIO_C + reg);
        }

        volatile constexpr static uint32_t& usart2(const uint32_t reg)
        {
            return peripheral(APB1_BASE + USART_2 + reg);
        }

        volatile constexpr static uint32_t& timer4(const uint32_t reg)
        {
            return peripheral(APB1_BASE + TIMER_4 + reg);
        }

        volatile constexpr static uint32_t& flash(const uint32_t reg)
        {
            return peripheral(AHB1_BASE + FLASH + reg);
        }

        volatile constexpr static uint32_t& pwr(const uint32_t reg)
        {
            return peripheral(APB1_BASE + PWR + reg);
        }

        volatile constexpr static uint32_t& scb(const uint32_t reg)
        {
            return peripheral(SCS_BASE + SCB + reg);
        }

        //
        // note, the nvic registers are accessed on 8 and 32 bit boundaries
        //

        volatile constexpr static uint32_t& nvic32(const uint32_t reg)
        {
            return peripheral(SCS_BASE + NVIC + reg);
        }

        volatile constexpr static uint8_t& nvic8(const uint32_t reg)
        {
            return *reinterpret_cast<uint8_t*>(SCS_BASE + NVIC + reg);
        }

    private:
        volatile constexpr static uint32_t& peripheral(const uint32_t address)
        {
            return *reinterpret_cast<uint32_t*>(address);
        }
};

#endif
