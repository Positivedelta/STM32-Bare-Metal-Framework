//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/f446xx_irq_n.hpp"
#include "framework/stm32f4/gpio.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/stm32f4/usart.hpp"

#include "framework/drivers/usart1.hpp"

driver::Usart1& driver::Usart1::getInstance()
{
    static auto instance = driver::Usart1();
    return instance;
}

// the lambda is used by the base class to enable the IRQ and then trigger the transmission of the first and subsequent bytes
//
driver::Usart1::Usart1():
    Uart([]() {
        Stm32f4::usart1(Usart::CR1) = Stm32f4::usart1(Usart::CR1) | Usart::CR1_TXEIE;
        (&Stm32f4::nvic32(Nvic::ISPR))[F446IRQn::Usart1 >> 5] = 1 << (F446IRQn::Usart1 & uint32_t(0x1f));
    }) {
}

// note, serial tx/rx port designations
// PA9: tx, PA10: rx
//
driver::Usart1& driver::Usart1::initialise(const bpl::BaudRate& baudRate, const uint8_t priority)
{
    // enable the gpioA and usart1 clocks
    //
    Stm32f4::rcc(Rcc::AHB1ENR) = Stm32f4::rcc(Rcc::AHB1ENR) | Rcc::AHB1ENR_GPIOA;
    Stm32f4::rcc(Rcc::APB2ENR) = Stm32f4::rcc(Rcc::APB2ENR) | Rcc::APB2ENR_USART1EN;

    // setup the appropriate pins on gpioA, PA9:PA10 --> tx:rx
    // note, pins default to push-pull
    //
    auto ospeeddr = Stm32f4::gpioA(Gpio::OSPEEDR);
    ospeeddr &= ~((Gpio::OSPEEDR_MASK << (Gpio::Pin10 << Gpio::OSPEEDR_SHIFT)) | (Gpio::OSPEEDR_MASK << (Gpio::Pin9 << Gpio::OSPEEDR_SHIFT)));
    ospeeddr |= ((Gpio::Fast << (Gpio::Pin10 << Gpio::OSPEEDR_SHIFT)) | (Gpio::Fast << (Gpio::Pin9 << Gpio::OSPEEDR_SHIFT)));
    Stm32f4::gpioA(Gpio::OSPEEDR) = ospeeddr;

    auto pupdr = Stm32f4::gpioA(Gpio::PUPDR);
    pupdr &= ~((Gpio::PUPDR_MASK << (Gpio::Pin10 << Gpio::PUPDR_SHIFT)) | (Gpio::PUPDR_MASK << (Gpio::Pin9 << Gpio::PUPDR_SHIFT)));
    pupdr |= ((Gpio::Pup << (Gpio::Pin10 << Gpio::PUPDR_SHIFT)) | (Gpio::Pup << (Gpio::Pin9 << Gpio::PUPDR_SHIFT)));
    Stm32f4::gpioA(Gpio::PUPDR) = pupdr;

    // gpioa requires the use of usart1 as an alternate function (AF7)
    //
    auto moder =  Stm32f4::gpioA(Gpio::MODER);
    moder &= ~((Gpio::MODER_MASK << (Gpio::Pin10 << Gpio::MODER_SHIFT)) | (Gpio::MODER_MASK << (Gpio::Pin9 << Gpio::MODER_SHIFT)));
    moder |= ((Gpio::AF << (Gpio::Pin10 << Gpio::MODER_SHIFT)) | (Gpio::AF << (Gpio::Pin9 << Gpio::MODER_SHIFT)));
    Stm32f4::gpioA(Gpio::MODER) = moder;

    // note, using gpio AFRH register for ports 8..15, these must be mapped to 0..7
    //
    const auto pin9 = Gpio::Pin9 - Gpio::Pin8;
    const auto pin10 = Gpio::Pin10 - Gpio::Pin8;
    auto afrh = Stm32f4::gpioA(Gpio::AFRH);
    afrh &= ~((Gpio::AFR_MASK << (pin10 << Gpio::AFR_SHIFT)) | (Gpio::AFR_MASK << (pin9 << Gpio::AFR_SHIFT)));
    afrh |= ((Gpio::AF7 << (pin10 << Gpio::AFR_SHIFT)) | (Gpio::AF7 << (pin9 << Gpio::AFR_SHIFT)));
    Stm32f4::gpioA(Gpio::AFRH) = afrh;

    // enable the receiver and transmitter
    //
    Stm32f4::usart1(Usart::CR1) = Usart::CR1_RE | Usart::CR1_TE;

    // configure the baud rate, usart1 is clocked 90 MHz on APB2
    // notes 1, CR2 and CR3 are left at their default values (stop bits, flow control...)
    //       2, [mantissa.divisor] = clock / (oversample:16|8 * baudRate) with .xxxx|.xxx used for the divisor
    //
    Stm32f4::usart1(Usart::BRR) = Uart::baudAPB2(baudRate);

    // enable the usart, enable the RX IRQ and begin with disabled TX IRQs (will be enabled by writes)
    //
    auto cr1 = Stm32f4::usart1(Usart::CR1);
    cr1 |= (Usart::CR1_UE | Usart::CR1_RXNEIE);
    cr1 &= ~Usart::CR1_TXEIE;
    Stm32f4::usart1(Usart::CR1) = cr1;

    // set the IRQ priority, hi --> low encoded as 0 --> 15 and enable the IRQ
    //
    Nvic::setPriority(F446IRQn::Usart1, priority);
    Nvic::enableIRQ(F446IRQn::Usart1);

    return *this;
}

__attribute__((flatten, hot)) void Usart1IRQ::handler()
{
    auto& instance = driver::Usart1::getInstance();

    // handle any rx IRQs, a read automatically clears the IRQ
    //
    if (Stm32f4::usart1(Usart::SR) & Usart::SR_RXNE)
    {
        const uint8_t byte = Stm32f4::usart1(Usart::DR);

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
    if (Stm32f4::usart1(Usart::SR) & Usart::SR_TXE)
    {
        // is there any data in the output buffer? if so, transmit it...
        //
        if (instance.outBufferHead != instance.outBufferTail)
        {
            Stm32f4::usart1(Usart::DR) = instance.outBuffer[instance.outBufferTail & (instance.OUT_BUFFER_SIZE - 1)];
            instance.outBufferTail = instance.outBufferTail + 1;
        }
        else
        {
            Stm32f4::usart1(Usart::CR1) = Stm32f4::usart1(Usart::CR1) & ~Usart::CR1_TXEIE;
        }
    }
}
