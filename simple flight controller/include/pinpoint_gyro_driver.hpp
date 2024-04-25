//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_PINPOINT_GYRO_DRIVER_H
#define BPL_PINPOINT_GYRO_DRIVER_H

#include "framework/cli/cli_provider.hpp"
#include "framework/tasking/task.hpp"

#include "led_driver.hpp"
#include "axes_values.hpp"

class PinPointGyroDriver : public bpl::Task, public bpl::CliProvider
{
    public:
        PinPointGyroDriver(const uint32_t period, const char* taskName, const LedDriver& led);
        void sample();
        const AxesValues getAxes();

    private:
        void runTask() override;
        bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, driver::Time& time) override;
};

#endif
