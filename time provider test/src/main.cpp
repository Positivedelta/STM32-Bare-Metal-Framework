//
// (c) Bit Parallel Ltd, April 2024
//

#include <cstdint>
#include <memory_resource>

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/gpio.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/drivers/time.hpp"
#include "framework/drivers/usart2.hpp"
#include "framework/io/baud_rate.hpp"
#include "framework/io/print_writer.hpp"

//
// note, the Nucleo can be configured to connect Usart2 to the virtual COM port on the ST-LINK module
//       this is very useful when running / testing this application
//

int main()
{
    // setup a PMR allocator for use with vectors, maps, ...
    // notes 1, for "full" dynamic allocation, chain an instance of std::pmr::unsynchronized_pool_resource(&upstreamMontonicResource)
    //       2, this application only allocates once, i.e. effectively using static allocation
    //
    auto pmrBuffer = std::array<uint8_t, 8192>();
    auto upstreamResource = std::pmr::null_memory_resource();
    auto pool = std::pmr::monotonic_buffer_resource(pmrBuffer.data(), pmrBuffer.size(), upstreamResource);
    std::pmr::set_default_resource(&pool);

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

    constexpr uint32_t clockFrequency = 90'000'000;
    auto& time = driver::Time::getInstance(clockFrequency, Nvic::Priority2);

    auto& uart = driver::Usart2::getInstance().initialise(bpl::BaudRate::BPS_115200, Nvic::Priority1);
    const auto& outputStream = uart.getOutputStream();
    const auto writer = bpl::PrintWriter(outputStream);
    writer.println();
    writer.println("Time Class Test");
    writer.println("Toggle the LED every second, print a '*' every minute...");

    auto count = uint32_t(0);
    const auto halfSecond = uint64_t(500'000);
    while (true)
    {
        const auto offTime = time.getMicros64();
        while (time.getMicros64() < offTime + halfSecond) asm volatile ("nop");
        Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5);

        const auto onTime = time.getMicros64();
        while (time.getMicros64() < onTime + halfSecond) asm volatile ("nop");
        Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);

        count++;
        if ((count % 60) == 0) writer.print("*");
    }

    return 0;
}
