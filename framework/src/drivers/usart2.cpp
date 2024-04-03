//
// (c) Bit Parallel Ltd, 2022
//

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/f446xx_irq_n.hpp"
#include "framework/stm32f4/gpio.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/stm32f4/usart.hpp"

#include "framework/drivers/usart2.hpp"

driver::Usart2& driver::Usart2::getInstance()
{
    static auto instance = driver::Usart2();
    return instance;
}

// the lambda is used by the base class to enable the IRQ and then trigger the transmission of the first and subsequent bytes
//
driver::Usart2::Usart2():
    Uart([]() {
        Stm32f4::usart2(Usart::CR1) = Stm32f4::usart2(Usart::CR1) | Usart::CR1_TXEIE;
        (&Stm32f4::nvic32(Nvic::ISPR))[F446IRQn::Usart2 >> 5] = 1 << (F446IRQn::Usart2 & uint32_t(0x1f));
    }) {
}

// note, serial tx/rx port designations
// PA2: tx, PA3: rx
//
driver::Usart2& driver::Usart2::initialise(const bpl::BaudRate& baudRate, const uint8_t priority)
{
    // enable the gpioA and usart2 clocks
    //
    Stm32f4::rcc(Rcc::AHB1ENR) = Stm32f4::rcc(Rcc::AHB1ENR) | Rcc::AHB1ENR_GPIOA;
    Stm32f4::rcc(Rcc::APB1ENR) = Stm32f4::rcc(Rcc::APB1ENR) | Rcc::APB1ENR_USART2EN;

    // setup the appropriate pins on gpioA, PA2:PA3 --> tx:rx
    // note, pins default to push-pull
    //
    auto ospeeddr = Stm32f4::gpioA(Gpio::OSPEEDR);
    ospeeddr &= ~((Gpio::OSPEEDR_MASK << (Gpio::Pin3 << Gpio::OSPEEDR_SHIFT)) | (Gpio::OSPEEDR_MASK << (Gpio::Pin2 << Gpio::OSPEEDR_SHIFT)));
    ospeeddr |= ((Gpio::Fast << (Gpio::Pin3 << Gpio::OSPEEDR_SHIFT)) | (Gpio::Fast << (Gpio::Pin2 << Gpio::OSPEEDR_SHIFT)));
    Stm32f4::gpioA(Gpio::OSPEEDR) = ospeeddr;

    auto pupdr = Stm32f4::gpioA(Gpio::PUPDR);
    pupdr &= ~((Gpio::PUPDR_MASK << (Gpio::Pin3 << Gpio::PUPDR_SHIFT)) | (Gpio::PUPDR_MASK << (Gpio::Pin2 << Gpio::PUPDR_SHIFT)));
    pupdr |= ((Gpio::Pup << (Gpio::Pin3 << Gpio::PUPDR_SHIFT)) | (Gpio::Pup << (Gpio::Pin2 << Gpio::PUPDR_SHIFT)));
    Stm32f4::gpioA(Gpio::PUPDR) = pupdr;

    // gpioa requires the use of usart2 as an alternate function (AF7)
    //
    auto moder =  Stm32f4::gpioA(Gpio::MODER);
    moder &= ~((Gpio::MODER_MASK << (Gpio::Pin3 << Gpio::MODER_SHIFT)) | (Gpio::MODER_MASK << (Gpio::Pin2 << Gpio::MODER_SHIFT)));
    moder |= ((Gpio::AF << (Gpio::Pin3 << Gpio::MODER_SHIFT)) | (Gpio::AF << (Gpio::Pin2 << Gpio::MODER_SHIFT)));
    Stm32f4::gpioA(Gpio::MODER) = moder;

    auto afrl = Stm32f4::gpioA(Gpio::AFRL);
    afrl &= ~((Gpio::AFR_MASK << (Gpio::Pin3 << Gpio::AFR_SHIFT)) | (Gpio::AFR_MASK << (Gpio::Pin2 << Gpio::AFR_SHIFT)));
    afrl |= ((Gpio::AF7 << (Gpio::Pin3 << Gpio::AFR_SHIFT)) | (Gpio::AF7 << (Gpio::Pin2 << Gpio::AFR_SHIFT)));
    Stm32f4::gpioA(Gpio::AFRL) = afrl;

    // enable the receiver and transmitter
    //
    Stm32f4::usart2(Usart::CR1) = Usart::CR1_RE | Usart::CR1_TE;

    // configure the baud rate, usart2 is clocked 45 MHz on APB1
    // notes 1, CR2 and CR3 are left at their default values (stop bits, flow control...)
    //       2, [mantissa.divisor] = clock / (oversample:16|8 * baudRate) with .xxxx|.xxx used for the divisor
    //
    Stm32f4::usart2(Usart::BRR) = Uart::baudAPB1(baudRate);

    // enable the usart, enable the RX IRQ and begin with disabled TX IRQs (will be enabled by writes)
    //
    auto cr1 = Stm32f4::usart2(Usart::CR1);
    cr1 |= (Usart::CR1_UE | Usart::CR1_RXNEIE);
    cr1 &= ~Usart::CR1_TXEIE;
    Stm32f4::usart2(Usart::CR1) = cr1;

    // set the IRQ priority, hi --> low encoded as 0 --> 15 and enable the IRQ
    //
    Nvic::setPriority(F446IRQn::Usart2, priority);
    Nvic::enableIRQ(F446IRQn::Usart2);

    return *this;
}

__attribute__((flatten, hot)) void Usart2IRQ::handler()
{
    auto& instance = driver::Usart2::getInstance();

    // handle any rx IRQs, a read automatically clear the IRQ
    //
    if (Stm32f4::usart2(Usart::SR) & Usart::SR_RXNE)
    {
        const uint8_t byte = Stm32f4::usart2(Usart::DR);

        // buffer the byte, will be discarded if the input buffer is full
        //
        if ((instance.inBufferHead - instance.inBufferTail) != instance.IN_BUFFER_SIZE)
        {
            instance.inBuffer[instance.inBufferHead & (instance.IN_BUFFER_SIZE - 1)] = byte;
            instance.inBufferHead = instance.inBufferHead + 1;
        }

        // pass the byte the registered listener
        // notes 1, if a listener has not been provided the byte will be consumed by the default handler
        //       2, the associated listener must be quick!
        //
        instance.wrappedListener(byte);
    }

    // handle the tx IRQ
    //
    if (Stm32f4::usart2(Usart::SR) & Usart::SR_TXE)
    {
        // is there any data in the output buffer? if so, transmit it...
        //
        if (instance.outBufferHead != instance.outBufferTail)
        {
            Stm32f4::usart2(Usart::DR) = instance.outBuffer[instance.outBufferTail & (instance.OUT_BUFFER_SIZE - 1)];
            instance.outBufferTail = instance.outBufferTail + 1;
        }
        else
        {
            Stm32f4::usart2(Usart::CR1) = Stm32f4::usart2(Usart::CR1) & ~Usart::CR1_TXEIE;
        }
    }
}
