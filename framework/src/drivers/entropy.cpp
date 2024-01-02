//
// (c) Bit Parallel Ltd, January 2024
//

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/f446xx_irq_n.hpp"
#include "framework/stm32f4/exti.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/stm32f4/pwr.hpp"
#include "framework/stm32f4/rtc.hpp"
#include "framework/stm32f4/timer.hpp"

#include "framework/drivers/entropy.hpp"

driver::Entropy& driver::Entropy::getInstance()
{
    static driver::Entropy instance = driver::Entropy();
    return instance;
}

driver::Entropy::Entropy() : cache(EntropyCache())
{
    //
    // RTC setup, used for its inaccurate 32kHz RC clock
    //

    Stm32f4::rcc(Rcc::APB1ENR) = Stm32f4::rcc(Rcc::APB1ENR) | Rcc::APB1ENR_PWREN;   // enable power control, required for the RTC
    Stm32f4::pwr(Pwr::CR) = Stm32f4::pwr(Pwr::CR) | Pwr::CR_DBP;                    // acccess to RTC and RTC backup registers & backup SRAM enabled

    Stm32f4::rcc(Rcc::CSR) = Stm32f4::rcc(Rcc::CSR) | Rcc::CSR_LSION;               // enable the LSI clock
    while ((Stm32f4::rcc(Rcc::CSR) & Rcc::CSR_LSIRDY) == 0);                        // FIXME! add a timeout?

    Stm32f4::rcc(Rcc::BDCR) = Stm32f4::rcc(Rcc::BDCR) | Rcc::BDCR_BDRST;
    Stm32f4::rcc(Rcc::BDCR) = Stm32f4::rcc(Rcc::BDCR) & ~Rcc::BDCR_BDRST;
    Stm32f4::rcc(Rcc::BDCR) = Stm32f4::rcc(Rcc::BDCR) | Rcc::BDCR_RTCSEL_1;         // use the LSI clock source
    Stm32f4::rcc(Rcc::BDCR) = Stm32f4::rcc(Rcc::BDCR) | Rcc::BDCR_RTCEN;            // enable the backup domain

    Stm32f4::rtc(Rtc::WPR) = 0xca;                                                  // enable write access to the RTC registers
    Stm32f4::rtc(Rtc::WPR) = 0x53;

    // the wakeup function is enabled through the WUTE bit in the RTC_CR register
    // the periodic wakeup interrupt is enabled by setting the WUTIE bit in the RTC_CR2 register
    //
    // the following sequence is required to configure or change the wakeup timer auto-reload value (WUT[15:0] in RTC_WUTR)
    //   1, clear WUTE in RTC_CR to disable the wakeup timer
    //   2, poll WUTWF until it is set in RTC_ISR to make sure the access to wakeup auto-reload counter and to WUCKSEL[2:0] bits is allowed
    //      it takes 1 to 2 RTCCLK clock cycles (due to clock synchronization)
    //   3, program the wakeup auto-reload value WUT[15:0] and the wakeup clock selection (WUCKSEL[2:0] bits in RTC_CR)
    //      set WUTE in RTC_CR to enable the timer again
    //      the wakeup timer restarts down-counting due to clock synchronization, the WUTWF bit is cleared up to 2 RTCCLK clocks cycles after WUTE is cleared
    //
    Stm32f4::rtc(Rtc::CR) = Stm32f4::rtc(Rtc::CR) & ~Rtc::CR_WUTE;
    while ((Stm32f4::rtc(Rtc::ISR) & Rtc::ISR_WUTWF) == 0);                         // FIXME! add a timeout?

    // count down in 10ms at the 62.5us update rate, the pool will fill in ~640ms, one value will be available in ~80ms
    //
    Stm32f4::rtc(Rtc::WUTR) = 0x00a0;
    Stm32f4::rtc(Rtc::ISR) = Stm32f4::rtc(Rtc::ISR) & ~Rtc::ISR_WUTF;

    Stm32f4::rtc(Rtc::CR) = Stm32f4::rtc(Rtc::CR) & ~Rtc::CR_WUCKSEL_MASK;          // mask out the lowest 3 bits for the write below
    Stm32f4::rtc(Rtc::CR) = Stm32f4::rtc(Rtc::CR) | 0x03;                           // 32kHz div 2, i.e. 62.5us per count
    Stm32f4::rtc(Rtc::CR) = Stm32f4::rtc(Rtc::CR) | Rtc::CR_WUTIE;                  // enable the wakeup IRQ
    Stm32f4::rtc(Rtc::CR) = Stm32f4::rtc(Rtc::CR) | Rtc::CR_WUTE;
    Stm32f4::rtc(Rtc::WPR) = 0xff;                                                  // re-enable write protect

    //
    // setup the 16-bit timer 3 peripheral, running as fast as possible and gets sampled by the RTC wakeup IRQ
    //

    Stm32f4::rcc(Rcc::APB1ENR) = Stm32f4::rcc(Rcc::APB1ENR) | Rcc::APB1ENR_TIM3EN;

    // timer off and reset
    //
    Stm32f4::timer3(Timer::CR1) = Stm32f4::timer3(Timer::CR1) & ~Timer::CR1_CEN;
    Stm32f4::rcc(Rcc::APB1RSTR) = Stm32f4::rcc(Rcc::APB1RSTR) | Rcc::APB1RSTR_TIM3RST;
    Stm32f4::rcc(Rcc::APB1RSTR) = Stm32f4::rcc(Rcc::APB1RSTR) & ~Rcc::APB1RSTR_TIM3RST;

    // FIXME! investigate further, clock set to be ~16 times faster than the RTC wakeup counter
    //
    Stm32f4::timer3(Timer::PSC) = 160;
    Stm32f4::timer3(Timer::CR1) = Stm32f4::timer3(Timer::CR1) | Timer::CR1_CEN;
}

driver::Entropy& driver::Entropy::enable(const uint32_t priority, const bool mustFill)
{
    // for RTC IRQs, must also enable the EXTI Line 22 and select rising edge sensitivity
    //
    Stm32f4::exti(Exti::IMR) = Stm32f4::exti(Exti::IMR) | Exti::IMR_MR22;
    Stm32f4::exti(Exti::RTSR) = Stm32f4::exti(Exti::RTSR) | Exti::RTSR_TR22;
    Nvic::setPriority(F446IRQn::RtcWkup, uint8_t(priority));
    Nvic::enableIRQ(F446IRQn::RtcWkup);

    // wait for the pool to fill, this will take ~640ms
    //
    if (mustFill) while (!isFull());

    return *this;
}

bool driver::Entropy::getRandom(uint32_t& value)
{
    if (isEmpty()) return false;
    Nvic::disableIRQ(F446IRQn::RtcWkup);

    const uint32_t wordIndex = cache.head / POOL_SIZE;
    value = cache.pool[wordIndex];

    // increment head by 8 nibbles, i.e. 32 bits
    //
    for (auto i = 0; i < 8; i++) incrementIndex(cache.head);
    cache.count = cache.count - 8;
    Nvic::enableIRQ(F446IRQn::RtcWkup);

    return true;
}

bool driver::Entropy::getBoundedRandom(uint32_t& value, const uint32_t upperBound)
{
    const auto status = getRandom(value);
    if (status) value = value % upperBound;

    return status;
}

bool driver::Entropy::getUniformBoundedRandom(uint32_t& value, const uint32_t upperBound)
{
    // skips values that would make (n % modulus) non-uniformly distributed
    // this is assuming that getRandom() itself is uniformly distributed from 0 to MAX_UINT32
    //
    auto status = false;
    while((status = getRandom(value)) && (value >  MAX_UINT32 - (MAX_UINT32 - upperBound - 1 ) % upperBound));
    if (status) value = value % upperBound;

    return status;
}

__attribute__((flatten, hot)) void RtcWkupIRQ::handler()
{
    // clear the IRQ
    //
    Stm32f4::rtc(Rtc::ISR) = ~Rtc::ISR_WUTF;    // rc_w0
    Stm32f4::exti(Exti::PR) = Exti::PR_PR22;    // rc_w1

    driver::Entropy::getInstance().appendToPool(Stm32f4::timer3(Timer::CNT));
}

//
// private methods
//

bool driver::Entropy::isEmpty() const
{
    // an entry is composed of 8 x 4 nibbles, i.e. 32 bits
    //
    return (cache.count < 8);
}

bool driver::Entropy::isFull() const
{
    if ((cache.head == 0) && (cache.tail == (cache.numberOfChunks - 1)))
    {
        return true;
    }
    else if (cache.tail == (cache.head - 1))
    {
        return true;
    }

    return false;
}

void driver::Entropy::incrementIndex(volatile uint8_t& index)
{
    if (index == cache.numberOfChunks - 1)
    {
        index = 0;
    }
    else
    {
        index = index + 1;
    }
}

void driver::Entropy::appendToPool(uint32_t value)
{
    if (isFull()) return;
    
    // append in 4-bit chunks
    //
    value &= 0x0f;
    const uint32_t wordIndex = cache.tail / POOL_SIZE;
    const uint32_t nibble = cache.tail % POOL_SIZE;
    const uint32_t mask = ~(0x0f << (nibble << 2));
    cache.pool[wordIndex] &= mask;
    cache.pool[wordIndex] |= (value << (nibble << 2));

    incrementIndex(cache.tail);
    cache.count = cache.count + 1;
}
