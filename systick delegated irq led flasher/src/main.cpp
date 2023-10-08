//
// (c) Bit Parallel Ltd, September 2023
//

#include <cstdint>
#include <memory_resource>
#include <vector>

#include "stm32.hpp"
#include "stm32_rcc.hpp"
#include "stm32_gpio.hpp"

#include "simple_callback.hpp"
#include "simple_timer.hpp"
#include "systick_handler.hpp"

int main()
{
    // setup a PMR allocator for use with vectors, maps, ...
    // notes 1, for "full" dynamic allocation, chain an instance of std::pmr::unsynchronized_pool_resource(&upstreamMontonicResource)
    //       2, this application only allocates once, i.e. effectively using static allocation
    //
    auto pmrBuffer = std::array<uint8_t, 8192>();
    auto upstreamResource = std::pmr::null_memory_resource();
    auto pool = std::pmr::monotonic_buffer_resource {pmrBuffer.data(), pmrBuffer.size(), upstreamResource};
    std::pmr::set_default_resource(&pool);

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

/*  //
    // start of test code...
    //

    double acc = 0;
    auto numbers = std::pmr::vector<double> {6.2, 2.6, 10.0, 10.0, 10.0, 10.0};
    for (auto number : numbers) acc += number;
    if (acc == 48.8)
    {
        Stm32::gpioA(Gpio::BSR) = 1 << Gpio::Pin5;
    }
    else
    {
        Stm32::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);
    }

    //
    // end of test code...
    // */

    //
    // notes 1, this example is slightly contrived in order to demonstrate two SimpleTimers with a SysTick IRQ dependency
    //       2, the ledSlowFlash callback simply flashes led2
    //       3, the ledOnToggle callback toggles the led2 on state, i.e. causes led2 for flash more quickly when on
    //

    volatile auto ledOn = false;
    bpl::SimpleCallBack ledSlowFlash = [&ledOn] {
        ledOn = !ledOn;
        Stm32::gpioA(Gpio::BSR) = (ledOn) ? (1 << Gpio::Pin5) : (1 << (Gpio::Pin5 + 16));
    };

    // whilst on, toggle led2 3 times
    //
    bpl::SimpleCallBack ledOnToggle = [&ledOn, toggleCount = 0, toggle = true] mutable {
        if (ledOn)
        {
            if (toggleCount++ < 6)
            {
                Stm32::gpioA(Gpio::BSR) = (toggle) ? (1 << (Gpio::Pin5 + 16)) : (1 << Gpio::Pin5);
                toggle = !toggle;
            }
            else
            {
                if ((Stm32::gpioA(Gpio::IDR) & (1 << Gpio::Pin5)) > 0) Stm32::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);
            }
        }
        else
        {
            toggleCount = 0;
            toggle = true;
        }
    };

    constexpr uint32_t TIME_BASE_US = 500;
    auto&& slowFlasher = bpl::SimpleTimer(2000 * TIME_BASE_US, ledSlowFlash);   // trigger every 1s
    auto&& onToggler = bpl::SimpleTimer(200 * TIME_BASE_US, ledOnToggle);       // trigget every 0.1s

//  auto sysTick = SysTick::getInstance(TIME_BASE_US, slowFlasher);
    auto sysTick = SysTick::getInstance(TIME_BASE_US, {slowFlasher, onToggler});
    sysTick.enable();

    // let the IRQs work their magic...
    //
    while (true) asm volatile ("nop");

    return 0;
}
