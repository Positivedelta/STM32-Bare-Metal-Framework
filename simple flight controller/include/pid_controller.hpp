//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_PID_CONTROLLER_H
#define BPL_PID_CONTROLLER_H

#include <cstdint>

#include "framework/cli/cli_provider.hpp"
#include "framework/tasking/task.hpp"

#include "pinpoint_gyro_driver.hpp"
#include "sbus_driver.hpp"
#include "pwm_servo_driver.hpp"
#include "led_driver.hpp"

class PidController : public bpl::Task, public bpl::CliProvider
{
    public:
        PidController(const uint32_t period, const char* taskName, const PinPointGyroDriver& gyros, const SBusDriver& sbus, const PWMServoDriver& servos, const LedDriver& led);

    protected:
        void runTask() override;
        bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter) override;
};

#endif
