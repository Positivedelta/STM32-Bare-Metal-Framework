//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_LED_DRIVER_H
#define BPL_LED_DRIVER_H

#include <cstdint>

#include "irq_consumer.hpp"

class LedDriver : public bpl::IrqConsumer
{
    private:
        const uint32_t timeBase;
        int32_t ledPeriod, ledPeriodCount;
        bool ledOn, ledFlashing, ledToggle;

    public:
        LedDriver(const uint32_t timeBase);

        void on();
        void off();
        void flash(uint32_t period);

        void irq() override;
        uint32_t getIrqRate() const override;
};

#endif
