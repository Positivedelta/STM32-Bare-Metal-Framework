//
// (c) Bit Parallel Ltd, 2022
//

#include "stm32.hpp"
#include "stm32_gpio.hpp"
#include "stm32_nvic.hpp"
#include "stm32_rcc.hpp"
#include "stm32_usart.hpp"
#include "stm32_f446xx_irq_n.hpp"

#include "usart2_handler.hpp"

void Usart2IRQ::handler()
{
    // handle any rx IRQs, a read automatically clear the IRQ
    //
    if (Stm32::usart2(Usart::SR) & Usart::SR_RXNE)
    {
        const uint8_t byte = Stm32::usart2(Usart::DR);

        // buffer the byte, will be discarded if the input buffer is full
        //
        if ((inBufferHead - inBufferTail) != IN_BUFFER_SIZE)
        {
            inBuffer[inBufferHead & (IN_BUFFER_SIZE - 1)] = byte;
            inBufferHead = inBufferHead + 1;
        }

        // pass the byte the registered listener
        // notes 1, if a listener has not been provided the byte will be consumed by the default handler
        //       2, the associated listener must be quick!
        //
        wrappedByteListener(byte);
    }

    // handle the tx IRQ
    //
    if (Stm32::usart2(Usart::SR) & Usart::SR_TXE)
    {
        // is there any data in the output buffer? if so, transmit it...
        //
        if (outBufferHead != outBufferTail)
        {
            Stm32::usart2(Usart::DR) = outBuffer[outBufferTail & (OUT_BUFFER_SIZE - 1)];
            outBufferTail = outBufferTail + 1;
        }
        else
        {
            Stm32::usart2(Usart::CR1) = Stm32::usart2(Usart::CR1) & ~Usart::CR1_TXEIE;
        }
    }
}

Usart2IRQ& Usart2IRQ::getInstance(const bpl::BaudRate& baudRate)
{
    static Usart2IRQ instance = Usart2IRQ(baudRate);
    return instance;
}

Usart2IRQ::Usart2IRQ(const bpl::BaudRate& baudRate): baudRate(baudRate), inputStream(Usart2IRQ::In()), outputStream(Usart2IRQ::Out())
{
    // enable the gpioA and usart2 clocks
    //
    Stm32::rcc(Rcc::AHB1ENR) = Stm32::rcc(Rcc::AHB1ENR) | Rcc::AHB1ENR_GPIOA;
    Stm32::rcc(Rcc::APB1ENR) = Stm32::rcc(Rcc::APB1ENR) | Rcc::APB1ENR_USART2EN;

    // setup the appropriate pins on gpioA, PA2:PA3 --> tx:rx
    // note, pins default to push-pull
    //
    Stm32::gpioA(Gpio::OSPEEDR) = Stm32::gpioA(Gpio::OSPEEDR) & ~((Gpio::OSPEEDR_MASK << (Gpio::Pin3 << Gpio::OSPEEDR_SHIFT)) | (Gpio::OSPEEDR_MASK << (Gpio::Pin2 << Gpio::OSPEEDR_SHIFT)));
    Stm32::gpioA(Gpio::OSPEEDR) = Stm32::gpioA(Gpio::OSPEEDR) | ((Gpio::Fast << (Gpio::Pin3 << Gpio::OSPEEDR_SHIFT)) | (Gpio::Fast << (Gpio::Pin2 << Gpio::OSPEEDR_SHIFT)));

    Stm32::gpioA(Gpio::PUPDR) = Stm32::gpioA(Gpio::PUPDR) & ~((Gpio::PUPDR_MASK << (Gpio::Pin3 << Gpio::PUPDR_SHIFT)) | (Gpio::PUPDR_MASK << (Gpio::Pin2 << Gpio::PUPDR_SHIFT)));
    Stm32::gpioA(Gpio::PUPDR) = Stm32::gpioA(Gpio::PUPDR) | ((Gpio::Pup << (Gpio::Pin3 << Gpio::PUPDR_SHIFT)) | (Gpio::Pup << (Gpio::Pin2 << Gpio::PUPDR_SHIFT)));

    // pins require the usart2 as an alternate function (AF7)
    //
    Stm32::gpioA(Gpio::MODER) = Stm32::gpioA(Gpio::MODER) & ~((Gpio::MODER_MASK << (Gpio::Pin3 << Gpio::MODER_SHIFT)) | (Gpio::MODER_MASK << (Gpio::Pin2 << Gpio::MODER_SHIFT)));
    Stm32::gpioA(Gpio::MODER) = Stm32::gpioA(Gpio::MODER) | ((Gpio::AF << (Gpio::Pin3 << Gpio::MODER_SHIFT)) | (Gpio::AF << (Gpio::Pin2 << Gpio::MODER_SHIFT)));

    Stm32::gpioA(Gpio::AFRL) = Stm32::gpioA(Gpio::AFRL) & ~((Gpio::AFR_MASK << (Gpio::Pin3 << Gpio::AFR_SHIFT)) | (Gpio::AFR_MASK << (Gpio::Pin2 << Gpio::AFR_SHIFT)));
    Stm32::gpioA(Gpio::AFRL) = Stm32::gpioA(Gpio::AFRL) | ((Gpio::AF7 << (Gpio::Pin3 << Gpio::AFR_SHIFT)) | (Gpio::AF7 << (Gpio::Pin2 << Gpio::AFR_SHIFT)));

    // enable the receiver and transmitter
    //
    Stm32::usart2(Usart::CR1) = Usart::CR1_RE | Usart::CR1_TE;

    // configure the baud rate when using a 45 MHz clock
    // note 1, CR2 and CR3 are left at their default values (stop bits, flow control...)
    //      2, [mantissa.divisor] = clock / (oversample:16|8 * baudRate) with .xxxx|.xxx used for the divisor
    //
    switch (baudRate)
    {
        case bpl::BaudRate::BPS_4800:
            // 585.9375 (closest fraction), i.e. 585 | 15 (has a 0% error)
            //
            Stm32::usart2(Usart::BRR) = (585 << 4) | 15;
            break;

        case bpl::BaudRate::BPS_9600:
            // 293.0000 (closest fraction), i.e. 292 | 16 (has a 0.011% error)
            //
            Stm32::usart2(Usart::BRR) = (292 << 4) | 16;
            break;

        case bpl::BaudRate::BPS_57600:
            // 48.8125 (closest fraction), i.e. 48 | 13 (has a -0.032% error)
            //
            Stm32::usart2(Usart::BRR) = (48 << 4) | 13;
            break;

        case bpl::BaudRate::BPS_115200:
            // 24.4375 (closest fraction), i.e. 24 | 7 (has an 0.096% error)
            //
            Stm32::usart2(Usart::BRR) = (24 << 4) | 7;
            break;

        case bpl::BaudRate::BPS_230400:
            // 12.1875 (closest fraction), i.e. 12 | 3 (has an 0.096% error)
            //
            Stm32::usart2(Usart::BRR) = (12 << 4) | 3;
            break;

        case bpl::BaudRate::BPS_460800:
            // 6.125 (closest fraction), i.e. 6 | 2 (has an 0.351% error)
            //
            Stm32::usart2(Usart::BRR) = (6 << 4) | 2;
            break;

        case bpl::BaudRate::BPS_921600:
            // 3.0625 (closest fraction), i.e. 3 | 1 (has an 0.351% error)
            //
            Stm32::usart2(Usart::BRR) = (3 << 4) | 1;
            break;
    }

    // enable the usart, enable the RX IRQ and begin with disabled TX IRQs (will be enabled by writes)
    //
    Stm32::usart2(Usart::CR1) = Stm32::usart2(Usart::CR1) | Usart::CR1_UE;
    Stm32::usart2(Usart::CR1) = Stm32::usart2(Usart::CR1) | Usart::CR1_RXNEIE;
    Stm32::usart2(Usart::CR1) = Stm32::usart2(Usart::CR1) & ~Usart::CR1_TXEIE;

    // set the IRQ priority, hi --> low encoded as 0 --> 15 and enable the IRQ
    //
    Nvic::setPriority(Stm32F446IRQn::Usart2, 1);
    Nvic::enableIRQ(Stm32F446IRQn::Usart2);
}

//
// Usart2IRQ::In inner class methods
//

const bool Usart2IRQ::In::read(uint8_t& byte) const
{
    // check that there is data in the input buffer?
    //
    bool success = false;
    if (inBufferHead != inBufferTail)
    {
        byte = inBuffer[inBufferTail & (IN_BUFFER_SIZE - 1)];
        inBufferTail = inBufferTail + 1;

        success = true;
    }

    return success;
}

// note, any registered listener must be quick as it will execute in the context of the IRQ handler
//
void Usart2IRQ::In::setByteListener(const bpl::ByteListener& byteListener) const
{
    wrappedByteListener = byteListener;
}

//
// Usart2IRQ::Out inner class methods
//

const bool Usart2IRQ::Out::write(const uint8_t byte) const
{
    // exit ASAP is the buffer is full, the supplied byte will be lost, i.e. not transmitted
    //
    if (bufferFull()) return false;

    outBuffer[outBufferHead & (OUT_BUFFER_SIZE - 1)] = byte;
    outBufferHead = outBufferHead + 1;

    // is this the 1st byte in the buffer?
    //
    if (outBufferHead - outBufferTail == 1)
    {
        // enable the IRQ and trigger the transmission of this and subsequent bytes
        //
        Stm32::usart2(Usart::CR1) = Stm32::usart2(Usart::CR1) | Usart::CR1_TXEIE;
        (&Stm32::nvic32(Nvic::ISPR))[Stm32F446IRQn::Usart2 >> 5] = 1 << (Stm32F446IRQn::Usart2 & uint32_t(0x1f));
    }

    return true;
}

const uint32_t Usart2IRQ::Out::write(const uint8_t bytes[], const uint32_t length) const
{
    for (uint32_t i = 0; i < length; i++)
    {
        if (write(bytes[i]) == false) return i;
    }

    return length;
}

const bool Usart2IRQ::Out::bufferFull() const
{
    return (outBufferHead - outBufferTail) == OUT_BUFFER_SIZE;
}

//
// the main Usart2IRQ class methods
//

const bpl::InputStream& Usart2IRQ::getInputStream()
{
    return inputStream;
}

const bpl::OutputStream& Usart2IRQ::getOutputStream()
{
    return outputStream;
}
