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
