//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_STM32F4_USART1_DRIVER_H
#define BPL_STM32F4_USART1_DRIVER_H

#include <cstdint>

#include "framework/drivers/uart.hpp"
#include "framework/io/baud_rate.hpp"

class Usart1IRQ
{
    public:
        static void handler();
};

namespace driver
{
    class Usart1 : public driver::Uart
    {
        friend void ::Usart1IRQ::handler();

        public:
            static Usart1& getInstance();
            Usart1& initialise(const bpl::BaudRate& baudRate, const uint8_t priority);

        private:
            Usart1();

        public:
            Usart1(const Usart1&) = delete;
            Usart1(const Usart1()&) = delete;
            Usart1& operator=(const Usart1&) = delete;
            void operator=(const Usart1()&) = delete;

            Usart1(Usart1&) = delete;
            Usart1(Usart1()&) = delete;
            Usart1& operator=(Usart1&) = delete;
            void operator=(Usart1()&) = delete;
    };
}

#endif
