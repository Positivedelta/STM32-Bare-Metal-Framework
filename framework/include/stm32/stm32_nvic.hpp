//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32NVIC_H
#define BPL_STM32NVIC_H

#include "stm32.hpp"

class Nvic
{
    public:
        inline static constexpr uint32_t ISER = 0x00;
        inline static constexpr uint32_t ICER = 0x80;
        inline static constexpr uint32_t ISPR = 0x100;
        inline static constexpr uint32_t ICPR = 0x180;
        inline static constexpr uint32_t IABR = 0x200;
        inline static constexpr uint32_t IP = 0x300;

        // IRQ priority group constants
        //
        inline static constexpr uint32_t PRE4_SUB0 = 0b011;

        // IRQ priority constants, PRI0..PRI15 ==> high..low
        //
        inline static constexpr uint8_t Priority0 = 0x00;
        inline static constexpr uint8_t Priority1 = 0x01;
        inline static constexpr uint8_t Priority2 = 0x02;
        inline static constexpr uint8_t Priority3 = 0x03;
        inline static constexpr uint8_t Priority4 = 0x04;
        inline static constexpr uint8_t Priority5 = 0x05;
        inline static constexpr uint8_t Priority6 = 0x06;
        inline static constexpr uint8_t Priority7 = 0x07;
        inline static constexpr uint8_t Priority8 = 0x08;
        inline static constexpr uint8_t Priority9 = 0x09;
        inline static constexpr uint8_t Priority10 = 0x0a;
        inline static constexpr uint8_t Priority11 = 0x0b;
        inline static constexpr uint8_t Priority12 = 0x0c;
        inline static constexpr uint8_t Priority13 = 0x0d;
        inline static constexpr uint8_t Priority14 = 0x0e;
        inline static constexpr uint8_t Priority15 = 0x0f;

        //
        // utility methods
        //

        static void enableIRQ(const uint32_t irqNumber)
        {
            // there are 32 IRQ enable bits per ISER register, arranaged sequentially
            // so, select the register based in the (IRQn / 32) and then the enable bit becomes IRQn & 0x1f (i.e. mod 32 via an AND)
            // note 1, no need to OR in these bits, a 0 write does nothing - there is a separate IRQ clear enable register (ICER)
            //      2, the [] forces 32-bit word addressing
            //
            (&Stm32::nvic32(Nvic::ISER))[irqNumber >> 5] = 1 << (irqNumber & uint32_t(0x1f));
            //Stm32::nvic32(Nvic::ISER + (Stm32IRQ::USART2_IRQn >> 4)) = 1 << (Stm32IRQ::USART2_IRQn & (uint32_t)0x1f);
        }

        static void setPriorityGrouping(const uint32_t priorityGrouping)
        {
            // note, as this register requires an unlock code, an intermediate variable must be used
            //
            volatile uint32_t value = Stm32::scb(Scb::AIRCR);
            value = value & (~(uint32_t(0xffff) << 16 | uint32_t(0b111) << 8));
            value = (((uint32_t)0x5fa << 16) | (priorityGrouping << 8));
            Stm32::scb(Scb::AIRCR) = value;
        }

        // note 1, the priority value may be encoded to represent group and sub-premeption components
        //         depends on the interpretation set using setNvicPriorityGrouping()
        //      2, the [] forces byte access to the register space i.e. each phyiscal 32-bit register contains 4 priorities for distinct IRQs
        //
        static void setPriority(const uint32_t irqNumber, const uint8_t priority)
        {
            (&Stm32::nvic8(Nvic::IP))[irqNumber] = priority << 4;
            //Stm32::nvic8(Nvic::IP + irqNumber) = priority << 4;
        }
};

#endif
