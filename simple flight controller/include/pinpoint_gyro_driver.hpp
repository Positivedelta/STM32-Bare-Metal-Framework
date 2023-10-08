//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_PINPOINT_GYRO_DRIVER_H
#define BPL_PINPOINT_GYRO_DRIVER_H

#include "irq_consumer.hpp"
#include "cli_provider.hpp"
#include "led_driver.hpp"
#include "axes_values.hpp"

class PinPointGyroDriver : public bpl::IrqConsumer, public CliProvider
{
    private:
        const uint32_t samplingInterval;

    public:
        PinPointGyroDriver(const uint32_t samplingInterval, const LedDriver& led);
        void sample();
        const AxesValues getAxes();

        void irq() override;
        uint32_t getIrqRate() const override;

    protected:
        bool doExecute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter) override;
};

#endif
