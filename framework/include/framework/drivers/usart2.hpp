//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_STM32F4_USART2_DRIVER_H
#define BPL_STM32F4_USART2_DRIVER_H

#include <cstdint>

#include "framework/drivers/uart.hpp"
#include "framework/io/baud_rate.hpp"

class Usart2IRQ
{
    public:
        static void handler();
};

namespace driver
{
    class Usart2 : public driver::Uart
    {
        friend void ::Usart2IRQ::handler();

        public:
            static Usart2& getInstance();
            Usart2& initialise(const bpl::BaudRate& baudRate, const uint8_t priority);

        private:
            Usart2();

        public:
            Usart2(const Usart2&) = delete;
            Usart2(const Usart2()&) = delete;
            Usart2& operator=(const Usart2&) = delete;
            void operator=(const Usart2()&) = delete;

            Usart2(Usart2&) = delete;
            Usart2(Usart2()&) = delete;
            Usart2& operator=(Usart2&) = delete;
            void operator=(Usart2()&) = delete;
    };
}

#endif
