//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_PID_CONTROLLER_H
#define BPL_PID_CONTROLLER_H

#include <cstdint>

#include "framework/cli/cli_provider.hpp"
#include "framework/radiocontrol/rc_input.hpp"
#include "framework/tasking/task.hpp"

#include "pinpoint_gyro_driver.hpp"
#include "pwm_servo_driver.hpp"
#include "led_driver.hpp"

class PidController : public bpl::Task, public bpl::CliProvider
{
    public:
        PidController(const uint32_t period, const char* taskName, const PinPointGyroDriver& gyros, const bpl::RcInput& rcInput, const PWMServoDriver& servos, const LedDriver& led);

    private:
        void runTask() override;
        bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console) override;
};

#endif
