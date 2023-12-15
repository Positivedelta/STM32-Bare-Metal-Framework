//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_PINPOINT_GYRO_DRIVER_H
#define BPL_PINPOINT_GYRO_DRIVER_H

#include "framework/tasking/task.hpp"

#include "cli_provider.hpp"
#include "led_driver.hpp"
#include "axes_values.hpp"

class PinPointGyroDriver : public bpl::Task, public CliProvider
{
    public:
        PinPointGyroDriver(const uint32_t period, const char* taskName, const LedDriver& led);
        void sample();
        const AxesValues getAxes();

    protected:
        void runTask() override;
        bool doExecute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter) override;
};

#endif
