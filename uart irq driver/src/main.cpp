//
// (c) Bit Parallel Ltd, September 2023
//

#include <cstdint>
#include <memory_resource>
#include <string>
#include <vector>

#include "stm32.hpp"
#include "stm32_gpio.hpp"
#include "stm32_nvic.hpp"
#include "stm32_rcc.hpp"

#include "baud_rate.hpp"
#include "usart2_handler.hpp"
#include "print_writer.hpp"
#include "text_reader.hpp"

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
    Stm32::rcc(Rcc::AHB1ENR) = Stm32::rcc(Rcc::AHB1ENR) | Rcc::AHB1ENR_GPIOA;
    Stm32::gpioA(Gpio::MODER) = Stm32::gpioA(Gpio::MODER) & (Gpio::MODER_MASK << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32::gpioA(Gpio::MODER) = Stm32::gpioA(Gpio::MODER) | (Gpio::OP << (Gpio::Pin5 << Gpio::MODER_SHIFT));
    Stm32::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);

    auto uart2 = Usart2IRQ::getInstance(bpl::BaudRate::BPS_115200);
    const auto& inputStream = uart2.getInputStream();
    const auto& outputStream = uart2.getOutputStream();

    auto lines = std::pmr::vector<std::pmr::string>();
    lines.emplace_back("The cat sat on the mat and drank from a saucer of milk");
    lines.emplace_back("The quick brown fox jumped over the lazy dog");
    lines.emplace_back("Shut the shutters and sit in the shop");

    // note, the PrintWriter::println() method is also overloaded to take C style char* values
    //
    const auto writer = bpl::PrintWriter(outputStream);
    writer.println();

    //
    // test the std::pmr::string APIs of the bpl::PrintWriter and bpl::TextReader classes
    //

    for (auto line : lines) writer.println(line);
    writer.println();
    writer.print("Please enter your name: ");

    // indicate uart activity by alternating the led2 state as characters
    // note, this is purely and example to demonstrate a possible use for a ByteListener
    //
    int32_t count = 0;
    const bpl::ByteListener listener = [&](const uint8_t rxedByte) {
        if (++count % 3) Stm32::gpioA(Gpio::ODR) = Stm32::gpioA(Gpio::ODR) ^ (1 << Gpio::Pin5);
    };

    inputStream.setByteListener(listener);

    // notes 1, readline() blocks, internally it uses WFI in order to sip power...
    //       2, the TextReader class can optionally take an OutputStream, this is to allow local echo
    //       3, use the templated TextReader::readln<N>(char(&textBuffer)[n]) method if you want to avoid allocating a std::pmr::string
    //
    const auto input = bpl::TextReader(inputStream, outputStream);
    std::string_view userName = input.readln();
    writer.print("Hello ");
    writer.print(userName);
    writer.println("!");

    writer.println();
    writer.println("Typed characters will now be echoed back to the terminal");
    while (true) input.readln();

/*  //
    // test the char* APIs of the bpl::PrintWriter and bpl::TextReader classes
    //

    for (auto line : lines) writer.println(line.c_str());
    writer.println();
    writer.print("Please enter your name: ");

    // indicate uart activity by alternating the led2 state as characters
    // note, this is purely and example to demonstrate a possible use for a ByteListener
    //
    int32_t count = 0;
    const bpl::ByteListener listener = [&](const uint8_t rxedByte) {
        if (++count % 3) Stm32::gpioA(Gpio::ODR) = Stm32::gpioA(Gpio::ODR) ^ (1 << Gpio::Pin5);
    };

    inputStream.setByteListener(listener);

    // notes 1, readline() blocks, internally it uses WFI in order to sip power...
    //       2, the TextReader class can optionally take an OutputStream, this is to allow local echo
    //       3, using the templated TextReader::readln<N>(char(&textBuffer)[n]) method to avoid allocating a std::pmr::string
    //
    const auto input = bpl::TextReader(inputStream, outputStream);

    const auto BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    input.readln<BUFFER_SIZE>(buffer);
    writer.print("Hello ");
    writer.print(buffer);
    writer.println("!");

    writer.println();
    writer.println("Typed characters will now be echoed back to the terminal");
    while (true) input.readln<BUFFER_SIZE>(buffer); */

    return 0;
}
