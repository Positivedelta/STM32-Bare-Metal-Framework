//
// (c) Bit Parallel Ltd, January 2024
//

#include <cstdint>

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/gpio.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/drivers/usart2.hpp"
#include "framework/drivers/entropy.hpp"
#include "framework/io/baud_rate.hpp"
#include "framework/io/print_writer.hpp"
#include "framework/utils/string_utils.hpp"

//
// note, the Nucleo can be configured to connect Usart2 to the virtual COM port on the ST-LINK module
//       this is very useful when running / testing this application
//

int main()
{
    // set the NVIC to use group4, i.e. 4 bits for preemption, 0 bits for sub priority
    //
    Nvic::setPriorityGrouping(Nvic::PRE4_SUB0);

    // enable the AHB1 clock required by gpioA and make PA5 an output, it's connected to led2
    // the led will be used as an input activity monitor, initialised off
    //
    Stm32f4::rcc(Rcc::AHB1ENR) = Stm32f4::rcc(Rcc::AHB1ENR) | Rcc::AHB1ENR_GPIOA;
    Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) & (Gpio::MODER_MASK << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) | (Gpio::OP << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);

    auto& uart = driver::Usart2::getInstance().initialise(bpl::BaudRate::BPS_115200, Nvic::Priority1);
    const auto& outputStream = uart.getOutputStream();
    const auto writer = bpl::PrintWriter(outputStream);

    // setting the IRQ priority lower than the uart
    // note, the Entropy class makes use of the RTC and Timer3
    //
    auto& entropy = driver::Entropy::getInstance();
    entropy.enable(Nvic::Priority2);

    const char* intro = "**** Random Number Generator ****";
    writer.println();
    writer.println(intro);
    writer.println();

    char buffer[8], unavailable[20] = "*** Unavailable!";
    uint32_t random = 0;
    while (true)
    {
        if (entropy.getUniformBoundedRandom(random, 1000))
        {
            writer.println(bpl::StringUtils::itoc<sizeof(buffer)>(random, buffer));
        }
        else
        {
            writer.println(unavailable);
        }

        // a crude but brief delay and LED flash...
        //
        Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5);
        for (auto i = 0; i < 2'500'000; i++) asm volatile ("nop");
        Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);
        for (auto i = 0; i < 2'500'000; i++) asm volatile ("nop");
    }

    return 0;
}
