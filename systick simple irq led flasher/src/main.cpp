//
// (c) Bit Parallel Ltd, 2021
//

#include <cstdint>

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/stm32f4/gpio.hpp"

#include "systick_delay_handler.hpp"

class LedFlasher
{
    private:
        SysTick& sysTick;

    public:
        LedFlasher(SysTick& sysTick) : sysTick(sysTick)
        {
            Stm32f4::gpioA(Gpio::BSR) = 1 << 5;
            sysTick.delay();
        }

        ~LedFlasher()
        {
            Stm32f4::gpioA(Gpio::BSR) = 1 << (5 + 16);
            sysTick.delay();
        }
};

int main()
{
    Stm32f4::rcc(Rcc::AHB1ENR) = Stm32f4::rcc(Rcc::AHB1ENR) | 1;            // enable the AHB1 clock, needed by gpioA
    asm("nop");

    // make PA5 an output, it's connected to led2
    // set led2 off as it's default state
    //
    Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) & ~(Gpio::MODER_MASK << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) | (Gpio::OP << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);

    // flash PA5...
    //
    const uint32_t delayMillis = 1000;
    SysTick sysTickDelay = SysTick(delayMillis);
    while (true)
    {
        const LedFlasher flash = LedFlasher(sysTickDelay);
    }

    return 0;
}
