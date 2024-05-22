//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_ADIS_IMU_DRIVER_H
#define BPL_ADIS_IMU_DRIVER_H

#include "framework/cli/cli_provider.hpp"
#include "framework/tasking/task.hpp"

#include "led_driver.hpp"
#include "axes_values.hpp"

class AdisImuDriver : public bpl::Task, public bpl::CliProvider
{
    public:
        AdisImuDriver(const uint32_t period, const char* taskName, const LedDriver& led);
        void sample();
        const AxesValues getAxes();

    private:
        void runTask() override;
        bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, driver::Time& time) override;
};

#endif
