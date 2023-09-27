//
// (c) Bit Parallel Ltd, August 2023
//

#include <cstdint>

#include "stm32.hpp"
#include "stm32_rcc.hpp"
#include "stm32_gpio.hpp"

void delay(uint64_t count)
{
    for (uint64_t i = 0; i < count; i++) asm("nop");
}

int main(void)
{
    Stm32::rcc(Rcc::AHB1ENR) = Stm32::rcc(Rcc::AHB1ENR) | (Rcc::AHB1ENR_GPIOA + Rcc::AHB1ENR_GPIOC);
    asm("nop");

    // make PA5 an output, it's connected to led2
    // set led2 off as it's default state
    //
    Stm32::gpioA(Gpio::MODER) = Stm32::gpioA(Gpio::MODER) & ~(Gpio::MODER_MASK << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32::gpioA(Gpio::MODER) = Stm32::gpioA(Gpio::MODER) | (Gpio::OP << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);

    // the blue push button is connected to gpioC PC13, it needs to be configured as an input
    // note, as (Gpio::IN is 0x00, there is no need to OR these bits in, i.e. only clear them using the mask
    //
    Stm32::gpioC(Gpio::MODER) = Stm32::gpioC(Gpio::MODER) & ~(Gpio::MODER_MASK << Gpio::Pin13 << Gpio::MODER_SHIFT);

    // flash PA5...
    //
    while (true)
    {
        if (!((Stm32::gpioC(Gpio::IDR) & (1 << Gpio::Pin13)) > 0))
        {
            Stm32::gpioA(Gpio::BSR) = 1 << Gpio::Pin5;
            delay(10000000u);

            Stm32::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);
            delay(10000000u);
        }
    }

    return 0;
}
