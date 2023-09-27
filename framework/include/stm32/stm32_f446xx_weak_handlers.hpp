//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32_F446XX_WEAK_HANDLERS_H
#define BPL_STM32_F446XX_WEAK_HANDLERS_H

class NMI
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class HardFault
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class MemManage
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class BusFault
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class UsageFault
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class SVCall
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class DebugMonitor
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class PendSV
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class SysTick
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class WwdgIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class PvdIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class TampStampIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class RtcWkupIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class FlashIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class RccIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Exti0IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Exti1IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Exti2IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Exti3IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Exti4IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma1Stream0IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma1Stream1IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma1Stream2IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma1Stream3IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma1Stream4IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma1Stream5IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma1Stream6IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class AdcIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Can1TxIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Can1Rx0IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Can1Rx1IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Can1SceIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Exti9To5IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim1BrkTim9IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim1UpTim10IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim1TrgComTim11IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim1CcIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim2IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim3IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim4IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class I2c1EvIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class I2c1ErIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class I2c2EvIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class I2c2ErIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Spi1IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Spi2IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Usart1IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Usart2IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Usart3IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Exti15To10IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class RtcAlarmIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class OtgFsWkup_IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim8BrkTim12IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim8UpTim13IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim8TrgComTim14IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class TiM8CcIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma1Stream7IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class FmcIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class SdioIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim5IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Spi3IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Uart4IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Uart5IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim6DacIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Tim7IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma2Stream0IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma2Stream1IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma2Stream2IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma2Stream3IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma2Stream4IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Can2TxIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Can2Rx0IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Can2Rx1IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Can2SceIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class OtgFsIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma2Stream5IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma2Stream6IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Dma2Stream7IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Usart6IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class I2c3EvIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class I2c3ErIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class OtgHsEp1OutIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class OtgHsEp1InIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class OtgHsWkupIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class OtgHsIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class DcmiIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class FpuIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Spi4IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Sai1IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class Sai2IRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class QuadSpiIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class CecIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class SpdifRxIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class FmpI2c1EventIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

class FmpI2c1ErrorIRQ
{
    public:
        __attribute__((weak)) static void handler()
        {
            while (true);
        }
};

#endif
