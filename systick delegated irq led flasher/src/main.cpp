//
// (c) Bit Parallel Ltd, September 2023
//

#include <cstdint>
#include <functional>
#include <memory_resource>
#include <vector>

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/gpio.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/tasking/task.hpp"
#include "framework/tasking/lambda_task.hpp"
#include "framework/tasking/scheduler.hpp"

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

    // set the NVIC to use group4, i.e. 4 bits for preemption, 0 bits for sub priority
    // note, this application only uses a small number of IRQ priority levels, i.e. no need for sub-levels
    //
    Nvic::setPriorityGrouping(Nvic::PRE4_SUB0);

    // enable the AHB1 clock, needed by gpioA
    //
    Stm32f4::rcc(Rcc::AHB1ENR) = Stm32f4::rcc(Rcc::AHB1ENR) | 1;
    asm("nop");

    // setup and initialise the Nucleo's user led2
    // make PA5 an output, it's connected to led2
    // set led2 off as it's default state
    //
    Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) & ~(Gpio::MODER_MASK << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) | (Gpio::OP << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);

/*  //
    // start of test code...
    //

    double acc = 0;
    auto numbers = std::pmr::vector<double> {6.2, 2.6, 10.0, 10.0, 10.0, 10.0};
    for (auto number : numbers) acc += number;
    if (acc == 48.8)
    {
        Stm32f4::gpioA(Gpio::BSR) = 1 << Gpio::Pin5;
    }
    else
    {
        Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);
    }

    //
    // end of test code...
    // */

    //
    // notes 1, this example is slightly contrived in order to demonstrate two scheduled tasks implemented using lambdas
    //       2, the ledSlowFlash task simply flashes led2
    //       3, the ledOnToggle task toggles the led2 on state, i.e. causes led2 for flash more quickly when on
    //

    volatile auto ledOn = false;
    auto ledSlowFlash = bpl::LambdaTask(1'000, "Slow Flash Task", [&ledOn] {
        ledOn = !ledOn;
        Stm32f4::gpioA(Gpio::BSR) = (ledOn) ? (1 << Gpio::Pin5) : (1 << (Gpio::Pin5 + 16));
    }); 

    // whilst on, toggle led2 3 times
    //
    auto ledOnToggle = bpl::LambdaTask(100, "On Toggle Task", [&ledOn, toggleCount = 0, toggle = true] mutable {
        if (ledOn)
        {
            if (toggleCount++ < 6)
            {
                Stm32f4::gpioA(Gpio::BSR) = (toggle) ? (1 << (Gpio::Pin5 + 16)) : (1 << Gpio::Pin5);
                toggle = !toggle;
            }
            else
            {
                if ((Stm32f4::gpioA(Gpio::IDR) & (1 << Gpio::Pin5)) > 0) Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);
            }
        }
        else
        {
            toggleCount = 0;
            toggle = true;
        }
    });

    // scheduled the slow flash to occur at 1Hz and the fast flash to occur at 10Hz
    //
    auto& scheduler = bpl::TaskScheduler::getInstance().initialise(1'000, {ledSlowFlash, ledOnToggle}, Nvic::Priority10);
    scheduler.start();

    // let the IRQs work their magic...
    //
    while (true) asm volatile ("nop");

    return 0;
}
