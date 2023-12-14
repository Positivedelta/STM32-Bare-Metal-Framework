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

driver::Usart2::Usart2():
    inBufferHead(0), inBufferTail(0), outBufferHead(0), outBufferTail(0), wrappedListener(nullListener),
    inputStream(Usart2::In(*this)), outputStream(Usart2::Out(*this)) {
}

driver::Usart2& driver::Usart2::initialise(const bpl::BaudRate& baudRate, const uint8_t priority)
{
    // enable the gpioA and usart2 clocks
    //
    Stm32f4::rcc(Rcc::AHB1ENR) = Stm32f4::rcc(Rcc::AHB1ENR) | Rcc::AHB1ENR_GPIOA;
    Stm32f4::rcc(Rcc::APB1ENR) = Stm32f4::rcc(Rcc::APB1ENR) | Rcc::APB1ENR_USART2EN;

    // setup the appropriate pins on gpioA, PA2:PA3 --> tx:rx
    // note, pins default to push-pull
    //
    Stm32f4::gpioA(Gpio::OSPEEDR) = Stm32f4::gpioA(Gpio::OSPEEDR) & ~((Gpio::OSPEEDR_MASK << (Gpio::Pin3 << Gpio::OSPEEDR_SHIFT)) | (Gpio::OSPEEDR_MASK << (Gpio::Pin2 << Gpio::OSPEEDR_SHIFT)));
    Stm32f4::gpioA(Gpio::OSPEEDR) = Stm32f4::gpioA(Gpio::OSPEEDR) | ((Gpio::Fast << (Gpio::Pin3 << Gpio::OSPEEDR_SHIFT)) | (Gpio::Fast << (Gpio::Pin2 << Gpio::OSPEEDR_SHIFT)));

    Stm32f4::gpioA(Gpio::PUPDR) = Stm32f4::gpioA(Gpio::PUPDR) & ~((Gpio::PUPDR_MASK << (Gpio::Pin3 << Gpio::PUPDR_SHIFT)) | (Gpio::PUPDR_MASK << (Gpio::Pin2 << Gpio::PUPDR_SHIFT)));
    Stm32f4::gpioA(Gpio::PUPDR) = Stm32f4::gpioA(Gpio::PUPDR) | ((Gpio::Pup << (Gpio::Pin3 << Gpio::PUPDR_SHIFT)) | (Gpio::Pup << (Gpio::Pin2 << Gpio::PUPDR_SHIFT)));

    // pins require the usart2 as an alternate function (AF7)
    //
    Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) & ~((Gpio::MODER_MASK << (Gpio::Pin3 << Gpio::MODER_SHIFT)) | (Gpio::MODER_MASK << (Gpio::Pin2 << Gpio::MODER_SHIFT)));
    Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) | ((Gpio::AF << (Gpio::Pin3 << Gpio::MODER_SHIFT)) | (Gpio::AF << (Gpio::Pin2 << Gpio::MODER_SHIFT)));

    Stm32f4::gpioA(Gpio::AFRL) = Stm32f4::gpioA(Gpio::AFRL) & ~((Gpio::AFR_MASK << (Gpio::Pin3 << Gpio::AFR_SHIFT)) | (Gpio::AFR_MASK << (Gpio::Pin2 << Gpio::AFR_SHIFT)));
    Stm32f4::gpioA(Gpio::AFRL) = Stm32f4::gpioA(Gpio::AFRL) | ((Gpio::AF7 << (Gpio::Pin3 << Gpio::AFR_SHIFT)) | (Gpio::AF7 << (Gpio::Pin2 << Gpio::AFR_SHIFT)));

    // enable the receiver and transmitter
    //
    Stm32f4::usart2(Usart::CR1) = Usart::CR1_RE | Usart::CR1_TE;

    // configure the baud rate when using a 45 MHz clock
    // note 1, CR2 and CR3 are left at their default values (stop bits, flow control...)
    //      2, [mantissa.divisor] = clock / (oversample:16|8 * baudRate) with .xxxx|.xxx used for the divisor
    //
    switch (baudRate)
    {
        case bpl::BaudRate::BPS_4800:
            // 585.9375 (closest fraction), i.e. 585 | 15 (has a 0% error)
            //
            Stm32f4::usart2(Usart::BRR) = (585 << 4) | 15;
            break;

        case bpl::BaudRate::BPS_9600:
            // 293.0000 (closest fraction), i.e. 292 | 16 (has a 0.011% error)
            //
            Stm32f4::usart2(Usart::BRR) = (292 << 4) | 16;
            break;

        case bpl::BaudRate::BPS_57600:
            // 48.8125 (closest fraction), i.e. 48 | 13 (has a -0.032% error)
            //
            Stm32f4::usart2(Usart::BRR) = (48 << 4) | 13;
            break;

        case bpl::BaudRate::BPS_115200:
            // 24.4375 (closest fraction), i.e. 24 | 7 (has an 0.096% error)
            //
            Stm32f4::usart2(Usart::BRR) = (24 << 4) | 7;
            break;

        case bpl::BaudRate::BPS_230400:
            // 12.1875 (closest fraction), i.e. 12 | 3 (has an 0.096% error)
            //
            Stm32f4::usart2(Usart::BRR) = (12 << 4) | 3;
            break;

        case bpl::BaudRate::BPS_460800:
            // 6.125 (closest fraction), i.e. 6 | 2 (has an 0.351% error)
            //
            Stm32f4::usart2(Usart::BRR) = (6 << 4) | 2;
            break;

        case bpl::BaudRate::BPS_921600:
            // 3.0625 (closest fraction), i.e. 3 | 1 (has an 0.351% error)
            //
            Stm32f4::usart2(Usart::BRR) = (3 << 4) | 1;
            break;
    }

    // enable the usart, enable the RX IRQ and begin with disabled TX IRQs (will be enabled by writes)
    //
    Stm32f4::usart2(Usart::CR1) = Stm32f4::usart2(Usart::CR1) | Usart::CR1_UE;
    Stm32f4::usart2(Usart::CR1) = Stm32f4::usart2(Usart::CR1) | Usart::CR1_RXNEIE;
    Stm32f4::usart2(Usart::CR1) = Stm32f4::usart2(Usart::CR1) & ~Usart::CR1_TXEIE;

    // set the IRQ priority, hi --> low encoded as 0 --> 15 and enable the IRQ
    //
    Nvic::setPriority(F446IRQn::Usart2, priority);
    Nvic::enableIRQ(F446IRQn::Usart2);

    return *this;
}

//
// Usart2::In inner class methods
//

driver::Usart2::In::In(Usart2& usart2):
    usart2(usart2) {
}

const bool driver::Usart2::In::read(uint8_t& byte) const
{
    // check that there is data in the input buffer?
    //
    if (usart2.inBufferHead != usart2.inBufferTail)
    {
        byte = usart2.inBuffer[usart2.inBufferTail & (IN_BUFFER_SIZE - 1)];
        usart2.inBufferTail = usart2.inBufferTail + 1;

        return true;
    }

    return false;
}

// note, any registered listener must be quick as it will execute in the context of the IRQ handler
//
void driver::Usart2::In::setByteListener(const bpl::ByteListener& listener) const
{
    usart2.wrappedListener = listener;
}

//
// Usart2::Out inner class methods
//

driver::Usart2::Out::Out(Usart2& usart2):
    usart2(usart2) {
}

const bool driver::Usart2::Out::write(const uint8_t byte) const
{
    // exit ASAP is the buffer is full, the supplied byte will be lost, i.e. not transmitted
    //
    if (bufferFull()) return false;

    usart2.outBuffer[usart2.outBufferHead & (OUT_BUFFER_SIZE - 1)] = byte;
    usart2.outBufferHead = usart2.outBufferHead + 1;

    // is this the 1st byte in the buffer?
    //
    if (usart2.outBufferHead - usart2.outBufferTail == 1)
    {
        // enable the IRQ and trigger the transmission of this and subsequent bytes
        //
        Stm32f4::usart2(Usart::CR1) = Stm32f4::usart2(Usart::CR1) | Usart::CR1_TXEIE;
        (&Stm32f4::nvic32(Nvic::ISPR))[F446IRQn::Usart2 >> 5] = 1 << (F446IRQn::Usart2 & uint32_t(0x1f));
    }

    return true;
}

const uint32_t driver::Usart2::Out::write(const uint8_t bytes[], const uint32_t length) const
{
    for (uint32_t i = 0; i < length; i++)
    {
        if (write(bytes[i]) == false) return i;
    }

    return length;
}

const bool driver::Usart2::Out::bufferFull() const
{
    return (usart2.outBufferHead - usart2.outBufferTail) == OUT_BUFFER_SIZE;
}

//
// the main Usart2 class methods
//

const bpl::InputStream& driver::Usart2::getInputStream() const
{
    return inputStream;
}

const bpl::OutputStream& driver::Usart2::getOutputStream() const
{
    return outputStream;
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
