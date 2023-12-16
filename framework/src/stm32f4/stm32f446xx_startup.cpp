//
// (c) Bit Parallel Ltd, 2021
//

#include <cstdint>

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/flash.hpp"
#include "framework/stm32f4/fpu.hpp"
#include "framework/stm32f4/gpio.hpp"
#include "framework/stm32f4/pwr.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/stm32f4/f446xx_weak_handlers.hpp"

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

__attribute__((weak)) extern void (*_spreinit_array[])(void);
__attribute__((weak)) extern void (*_epreinit_array[])(void);
__attribute__((weak)) extern void (*_sinit_array[])(void);
__attribute__((weak)) extern void (*_einit_array[])(void);

int main();

// minimal implemenation stubs required by libstdc++_nano
//
extern "C"
{
    void _exit(int status)
    {
        // something unexpected / bad has happened, let the user know...
        //
        // use the LED on PA5 to indicate to the outside world that exit() has been invoked
        // note, this code is NucleoF446RE specific
        //
        Stm32f4::rcc(Rcc::AHB1ENR) = Stm32f4::rcc(Rcc::AHB1ENR) | Rcc::AHB1ENR_GPIOA;
        Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) & ~(Gpio::MODER_MASK << (Gpio::Pin5 << Gpio::MODER_SHIFT));
        Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) | (Gpio::OP << (Gpio::Pin5 << Gpio::MODER_SHIFT));

        // turn the led on and then lock-up...
        //
        Stm32f4::gpioA(Gpio::BSR) = 1 << Gpio::Pin5;
        while (true);
    }

    // note, _heap_end is defined in the linker script
    //
    uint8_t* _sbrk(int32_t incr)
    {
        extern uint32_t _heap_end;
        static uint32_t heapEnd;
        uint32_t prevHeapEnd;

        if (heapEnd == 0) heapEnd = (uint32_t)&_heap_end;
        prevHeapEnd = heapEnd;

        // check for heap and stack overlap, but what to do?
        //
        uint32_t stackPointer;
        asm ("mov %0, sp" : "=r" (stackPointer));
        if (heapEnd + incr > stackPointer) _exit(-1);

        heapEnd += incr;
        return (uint8_t*)prevHeapEnd;
    }

    int32_t _kill(int32_t pid, int32_t sig)
    {
        return -1;
    }

    int32_t _getpid()
    {
        return 1;
    }

    //
    // added to support the "_printf_float" linker option
    //

    int32_t _isatty(int32_t file)
    {
        return 1;
    }

    int32_t _lseek(int32_t file, int32_t ptr, int32_t dir)
    {
        return 0;
    }

    int32_t _fstat(int32_t file, struct stat* st)
    {
        return 0;
    }

    int32_t _close(int32_t file)
    {
        return -1;
    }

    // notes 1, to support std::scanf() uncomment the code below and add and external reference to uartGetch()
    //       2, in main() implement uartGetch()
    //
    int32_t _read(int32_t fd, char* data, int32_t length)
    {
        return -1;

/*      if (fd != 0) return -1;

        int32_t bytesRead = 0;
        for (; length > 0; --length)
        {
            if (!uartGetch(data++)) break;
            bytesRead++;
        }

        return bytesRead; */
    }

    // notes 1, to support std::printf() uncomment the code below and add and external reference to uartPutch()
    //       2, in main() implement uartPutch()
    //
    int32_t _write(int32_t fd, char* text, int32_t length)
    {
        return -1;

/*      if ((fd != 1) && (fd != 2) && (fd != 3)) return -1;

        int32_t bytesWritten = 0;
        for (; length != 0; --length)
        {
            if (!uartPutch(*text++)) return -1;
            ++bytesWritten;
        }

        return bytesWritten; */
    }
}

class PowerOnReset
{
    private:
        enum class ClockStatus
        {
            Success,
            WaitStateFail,
            SwitchToHseFail,
            EnableOverdriveFail,
            SwitchToOverdriveFail,
            MainPllLockFail,
            SystemMultiplexerFail,
            SaiPllLockFail
        };

    public:
        static void handler()
        {
            // reconfigure the system clock to run at 180 MHz
            //
            if (configureClocks() != ClockStatus::Success)
            {
                // use the LED on PA5 to indicate to the outside world that there was a problem reconfiguring the clocks
                // note, this code is NucleoF446RE specific
                //
                Stm32f4::rcc(Rcc::AHB1ENR) = Stm32f4::rcc(Rcc::AHB1ENR) | Rcc::AHB1ENR_GPIOA;
                Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) & ~(Gpio::MODER_MASK << (Gpio::Pin5 << Gpio::MODER_SHIFT));
                Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) | (Gpio::OP << (Gpio::Pin5 << Gpio::MODER_SHIFT));

                // FIXME! use the return type to indicate the failure reason to the user
                // turn the led on and then lock-up...
                //
                Stm32f4::gpioA(Gpio::BSR) = 1 << Gpio::Pin5;
                while (true);
            }

            uint32_t *src = &_sidata;
            uint32_t *dst = &_sdata;

            // copy initialised data and clear the bss section
            //
            while (dst < &_edata) *(dst++) = *(src++);

            dst = &_sbss;
            while (dst < &_ebss) *(dst++) = 0;

            // call the c++ pre-initialisers
            //
            const uint32_t cppPreSize = &(_epreinit_array[0]) - &(_spreinit_array[0]);
            for (uint32_t cppCount = 0; cppCount < cppPreSize; ++cppCount) _spreinit_array[cppCount]();

            // call the initialisers, static constructors
            //
            const uint32_t cppInitSize = &(_einit_array[0]) - &(_sinit_array[0]);
            for (uint32_t cppCount = 0; cppCount < cppInitSize; ++cppCount) _sinit_array[cppCount]();

            // enable the fpu
            //
            Stm32f4::fpu(Fpu::CPACR) = Stm32f4::fpu(Fpu::CPACR) | Fpu::CPACR_CP10_FULL_ACCESS | Fpu::CPACR_CP11_FULL_ACCESS;

            // call main() but don't expect to exit!
            //
            main();
            while (true);

            //
            // note 1, not calling the .fini_array tidy up functions as this code would never be reached...
            //      2, unless the above while loop envelops main()
            //
        }

    // note, the Stm32f4::AHB_CLOCK constant is set to 180 MHz (i.e. 180000000)
    //
    private:
        static ClockStatus configureClocks()
        {
            // configure the flash wait states, set to 5 (6 CPU cycles), 150 MHz < HCLK <= 180 MHz @2.7v to 3.6v, required for the 180 MHz clock speed
            // also enable the data cache and instruction caches as well as the instruction prefetch pipeline
            // then verify that the wait states were accepted? (as recommended by the datasheet)
            //
            Stm32f4::flash(Flash::ACR) = 5 | Flash::ACR_DCEN | Flash::ACR_ICEN | Flash::ACR_PRFTEN;
            if ((Stm32f4::flash(Flash::ACR) & Flash::ACR_LATENCY_MASK) != 5) return ClockStatus::WaitStateFail;

            // enable the use of the external clock, connected to a 24 MHz oscillator
            // then wait for the oscillator to stabilise, after ~100 ms raise a warning or error
            //
            uint16_t watchdogCount = 0xffff;
            Stm32f4::rcc(Rcc::CR) = Stm32f4::rcc(Rcc::CR) | Rcc::CR_HSEON;
            while ((Stm32f4::rcc(Rcc::CR) & Rcc::CR_HSERDY) == 0) if (--watchdogCount == 0) return ClockStatus::SwitchToHseFail;

            // as the 180 MHz maximum clock option is about to be selected, enable the required power control overdrive mode
            // notes 1, enable the power control clock
            //       2, set ODEN bit of PWR_CR register to enable the Over-drive mode and wait for the ODRDY flag to be set in the PWR_CSR register
            //       3, set the ODSWEN bit in the PWR_CR register to switch the voltage regulator from Normal mode to Over-drive mode, wait for the ODSWRDY flag in the PWR_CSR to be set
            //          the System will be stalled during the switch but the PLL clock system will be still running during locking phase
            //       4, this process can be overlapped with the PPL locking phase during its setup
            //
            Stm32f4::rcc(Rcc::APB1ENR) = Stm32f4::rcc(Rcc::APB1ENR) | Rcc::APB1ENR_PWREN;
            Stm32f4::pwr(Pwr::CR) = Stm32f4::pwr(Pwr::CR) | Pwr::CR_ODEN;
            watchdogCount = 0xffff;
            while ((Stm32f4::pwr(Pwr::CSR) & Pwr::CSR_ODRDY) == 0) if (--watchdogCount == 0) return ClockStatus::EnableOverdriveFail;

            Stm32f4::pwr(Pwr::CR) = Stm32f4::pwr(Pwr::CR) | Pwr::CR_ODSWEN;
            watchdogCount = 0xffff;
            while ((Stm32f4::pwr(Pwr::CSR) & Pwr::CSR_ODSWRDY) == 0) if (--watchdogCount == 0) return ClockStatus::SwitchToOverdriveFail;

            // main PLL config, then enable
            //   1, select the external 24 MHz oscillator (HSE)
            //   2, set the PLL input to the recommended 2 Mhz, i.e. divide the 24 Mhz by 12 (PLLM)
            //   3, set the main multiplier to 180 (PLLN), i.e. generate 336 MHz, will get divided by 2 to generate a 180 MHz system clock
            //   4, divide by 2 (PLLP) the PLL to generate the 180 MHz system clock, this is the default value (i.e. no associated | term below)
            //   5, set the division factor (PLLQ) to 8, generates a safe 45 MHz clock... this will not be used, the USB clocks is generated by the SAI PLL (see below)
            //   6, set the division factor (PLLR) for I2Ss, SAIs, SYSTEM and SPDIF-Rx clocks, 336 / 2 = 180 MHz
            //   7, wait for the PLL to lock
            //
            Stm32f4::rcc(Rcc::PLLCFGR) = Rcc::PLLCFGR_PLLSRC | 12 | (180 << Rcc::PLLCFGR_PLLN_SHIFT) | (8 << Rcc::PLLCFGR_PLLQ_SHIFT) | (2 << Rcc::PLLCFGR_PLLR_SHIFT);
            Stm32f4::rcc(Rcc::CR) = Stm32f4::rcc(Rcc::CR) | Rcc::CR_PLLON;
            watchdogCount = 0xffff;
            while ((Stm32f4::rcc(Rcc::CR) & Rcc::CR_PLLRDY) == 0) if (--watchdogCount == 0) return ClockStatus::MainPllLockFail;

            // configure the system clock multiplexor and the remaining main bus clocks
            //   1, use the default AHB prescaler value of 1 (HPRE)
            //   2, set PPRE1 to 5, i.e. APB1 clock = 180 / 4 = 45 MHz
            //   3, set PPRE2 to 4, i.e. APB2 clock = 180 / 2 = 90 MHz
            //   4, set PLL_P (SW) as the selected system clock, i.e. configure the system clock multiplexer
            //   5, wait / verify for the main PLL to be selected by the system clock multiplexer
            //
            Stm32f4::rcc(Rcc::CFGR) = (5 << Rcc::CFGR_PPRE1_SHIFT) | (4 << Rcc::CFGR_PPRE2_SHIFT) | 2;
            watchdogCount = 0xffff;
            while ((Stm32f4::rcc(Rcc::CFGR) & (3 << Rcc::CFGR_SWS_SHIFT)) != (2 << Rcc::CFGR_SWS_SHIFT)) if (--watchdogCount == 0) return ClockStatus::SystemMultiplexerFail;

            // setup and enable the SAI PLL, will use it to setup the 48 MHz USB clock (can't use the main PLL when running at 180 MHz to do this)
            //  1, divide the input by 12 (PLLSAIM), i.e. feed the PLL with 2 MHz
            //  2, set the main multiplier to 192 (PLLSAIN), generates an output of 384 MHz
            //  3, divide the 384 MHz VCO output by 8 (PLLSAIP = 3) to generate 48 MHz (for use with USB if required)
            //  4, generate the PLLSAIQ output, divide the VCO by 8 yeilding 48 MHz, note this is not going to be used so may need to be revised if this changes...
            //  5, wait for the SAI PLL to lock
            //
            Stm32f4::rcc(Rcc::PLLSAICFGR) = 12 | (192 << Rcc::PLLSAICFGR_PLLSAIN_SHIFT) | (3 << Rcc::PLLSAICFGR_PLLSAIP_SHIFT) | (8 << Rcc::PLLSAICFGR_PLLSAIQ_SHIFT);
            Stm32f4::rcc(Rcc::CR) = Stm32f4::rcc(Rcc::CR) | Rcc::CR_PLLSAION;
            watchdogCount = 0xffff;
            while ((Stm32f4::rcc(Rcc::CR) & Rcc::CR_PLLSAIRDY) == 0) if (--watchdogCount == 0) return ClockStatus::SaiPllLockFail;

            // select the USB clock to come from the SAI PPL, not the main PLL (this sets the PLL48CLK Mux, as displayed in CubeMX)
            //
            Stm32f4::rcc(Rcc::DCKCFGR2) = Stm32f4::rcc(Rcc::DCKCFGR2) | Rcc::DCKCFGR2_CK48MSEL;

            // it's all over... happy!
            //
            return ClockStatus::Success;
        }
};

// FIXME! investigate how to use (*const vectors[]) and have them go the the .vector_table section and not .rodata
//
// function pointer array for the vector table
//
void (*vectors[])(void) __attribute__ ((section(".vector_table"))) = {
    reinterpret_cast<void(*)()>(&_estack),
    PowerOnReset::handler,
    NMI::handler,
    HardFault::handler,
    MemManage::handler,
    BusFault::handler,
    UsageFault::handler,
    0, 0, 0, 0,
    SVCall::handler,
    DebugMonitor::handler,
    0,
    PendSV::handler,
    SysTick::handler,
    WwdgIRQ::handler,
    PvdIRQ::handler,
    TampStampIRQ::handler,
    RtcWkupIRQ::handler,
    FlashIRQ::handler,
    RccIRQ::handler,
    Exti0IRQ::handler,
    Exti1IRQ::handler,
    Exti2IRQ::handler,
    Exti3IRQ::handler,
    Exti4IRQ::handler,
    Dma1Stream0IRQ::handler,
    Dma1Stream1IRQ::handler,
    Dma1Stream2IRQ::handler,
    Dma1Stream3IRQ::handler,
    Dma1Stream4IRQ::handler,
    Dma1Stream5IRQ::handler,
    Dma1Stream6IRQ::handler,
    AdcIRQ::handler,
    Can1TxIRQ::handler,
    Can1Rx0IRQ::handler,
    Can1Rx1IRQ::handler,
    Can1SceIRQ::handler,
    Exti9To5IRQ::handler,
    Tim1BrkTim9IRQ::handler,
    Tim1UpTim10IRQ::handler,
    Tim1TrgComTim11IRQ::handler,
    Tim1CcIRQ::handler,
    Tim2IRQ::handler,
    Tim3IRQ::handler,
    Tim4IRQ::handler,
    I2c1EvIRQ::handler,
    I2c1ErIRQ::handler,
    I2c2EvIRQ::handler,
    I2c2ErIRQ::handler,
    Spi1IRQ::handler,
    Spi2IRQ::handler,
    Usart1IRQ::handler,
    Usart2IRQ::handler,
    Usart3IRQ::handler,
    Exti15To10IRQ::handler,
    RtcAlarmIRQ::handler,
    OtgFsWkup_IRQ::handler,
    Tim8BrkTim12IRQ::handler,
    Tim8UpTim13IRQ::handler,
    Tim8TrgComTim14IRQ::handler,
    TiM8CcIRQ::handler,
    Dma1Stream7IRQ::handler,
    FmcIRQ::handler,
    SdioIRQ::handler,
    Tim5IRQ::handler,
    Spi3IRQ::handler,
    Uart4IRQ::handler,
    Uart5IRQ::handler,
    Tim6DacIRQ::handler,
    Tim7IRQ::handler,
    Dma2Stream0IRQ::handler,
    Dma2Stream1IRQ::handler,
    Dma2Stream2IRQ::handler,
    Dma2Stream3IRQ::handler,
    Dma2Stream4IRQ::handler,
    0, 0,
    Can2TxIRQ::handler,
    Can2Rx0IRQ::handler,
    Can2Rx1IRQ::handler,
    Can2SceIRQ::handler,
    OtgFsIRQ::handler,
    Dma2Stream5IRQ::handler,
    Dma2Stream6IRQ::handler,
    Dma2Stream7IRQ::handler,
    Usart6IRQ::handler,
    I2c3EvIRQ::handler,
    I2c3ErIRQ::handler,
    OtgHsEp1OutIRQ::handler,
    OtgHsEp1InIRQ::handler,
    OtgHsWkupIRQ::handler,
    OtgHsIRQ::handler,
    DcmiIRQ::handler,
    0, 0,
    FpuIRQ::handler,
    0, 0,
    Spi4IRQ::handler,
    0, 0,
    Sai1IRQ::handler,
    0, 0, 0,
    Sai2IRQ::handler,
    QuadSpiIRQ::handler,
    CecIRQ::handler,
    SpdifRxIRQ::handler,
    FmpI2c1EventIRQ::handler,
    FmpI2c1ErrorIRQ::handler
};
