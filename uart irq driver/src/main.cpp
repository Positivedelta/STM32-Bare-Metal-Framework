//
// (c) Bit Parallel Ltd, September 2023
//

#include <cstdint>
#include <memory_resource>
#include <string>
#include <vector>

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/gpio.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/drivers/usart2.hpp"
#include "framework/io/baud_rate.hpp"
#include "framework/io/print_writer.hpp"
#include "framework/io/text_reader.hpp"

// note, required by the char* version of TextReader::readln()
//
#include "framework/io/editproviders/char_array_edit_buffer.hpp"

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

    auto& uart = driver::Usart2::getInstance().initialise(bpl::BaudRate::BPS_115200, Nvic::Priority1);
    const auto& inputStream = uart.getInputStream();
    const auto& outputStream = uart.getOutputStream();

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
    // notes 1, this is purely and example to demonstrate a possible use for a ByteListener
    //       2, it returns false to allow the uart IRQ handler to continue processing the RXed byte
    //
    int32_t count = 0;
    const bpl::ByteListener listener = [&](const uint8_t rxedByte) {
        if (++count % 3) Stm32f4::gpioA(Gpio::ODR) = Stm32f4::gpioA(Gpio::ODR) ^ (1 << Gpio::Pin5);
        return false;
    };

    uart.setByteListener(listener);

    // notes 1, readln() blocks, internally it uses WFI in order to sip power...
    //       2, the readln() method also supports history, just override the default edit provider
    //       3, the TextReader class can optionally take an OutputStream, this is to allow local echo
    //       4, use the templated TextReader::readln<N>(char(&textBuffer)[n]) method if you want to avoid allocating a std::pmr::string
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
        if (++count % 3) Stm32f4::gpioA(Gpio::ODR) = Stm32f4::gpioA(Gpio::ODR) ^ (1 << Gpio::Pin5);
    };

    uart.setByteListener(listener);

    // notes 1, readline() blocks, internally it uses WFI in order to sip power...
    //       2, the TextReader class can optionally take an OutputStream, this is to allow local echo
    //       3, using the templated TextReader::readln<N>(char(&textBuffer)[n]) method to avoid allocating a std::pmr::string
    //
    const auto input = bpl::TextReader(inputStream, outputStream);

    const auto BUFFER_SIZE = 1024;
    auto editProvider = bpl::CharArrayEditBuffer<BUFFER_SIZE>();
    input.readln<BUFFER_SIZE>(editProvider);
    writer.print("Hello ");
    writer.print(editProvider.buffer());
    writer.println("!");

    writer.println();
    writer.println("Typed characters will now be echoed back to the terminal");
    while (true) input.readln<BUFFER_SIZE>(editProvider); */

    return 0;
}
