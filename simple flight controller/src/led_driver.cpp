//
// (c) Bit Parallel Ltd, October 2023
//

#include "stm32.hpp"
#include "stm32_rcc.hpp"
#include "stm32_gpio.hpp"

#include "led_driver.hpp"

LedDriver::LedDriver(const uint32_t timeBase):
    timeBase(timeBase), ledPeriod(1), ledPeriodCount(0), ledOn(false), ledFlashing(false), ledToggle(true) {
        // enable the AHB1 clock, needed by gpioA
        //
        Stm32::rcc(Rcc::AHB1ENR) = Stm32::rcc(Rcc::AHB1ENR) | 1;
        asm("nop");

        // setup and initialise the Nucleo's user led2
        // make PA5 an output, it's connected to led2
        // set led2 off as it's default state
        //
        Stm32::gpioA(Gpio::MODER) = Stm32::gpioA(Gpio::MODER) & ~(Gpio::MODER_MASK << (Gpio::Pin5 << Gpio::MODER_SHIFT));
        Stm32::gpioA(Gpio::MODER) = Stm32::gpioA(Gpio::MODER) | (Gpio::OP << (Gpio::Pin5 << Gpio::MODER_SHIFT));
        Stm32::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);
}

void LedDriver::on()
{
    ledFlashing = false;
    ledToggle = false;
    ledOn = true;
}

void LedDriver::off()
{
    ledFlashing = false;
    ledToggle = true;
    ledOn = false;
}

void LedDriver::flash(uint32_t period)
{
    ledPeriod = (period == 0) ? 1 : period;
    ledPeriodCount = 0;
    ledFlashing = true;
}

void LedDriver::irq()
{
    if (ledFlashing)
    {
        if (++ledPeriodCount == ledPeriod)
        {
            Stm32::gpioA(Gpio::BSR) = 1 << ((ledToggle) ? Gpio::Pin5 : Gpio::Pin5 + 16);
            ledToggle = !ledToggle;

            ledPeriodCount = 0;
        }

        return;
    }

    Stm32::gpioA(Gpio::BSR) = 1 << ((ledOn) ? Gpio::Pin5 : Gpio::Pin5 + 16);
}

uint32_t LedDriver::getIrqRate() const
{
    return timeBase;
}
