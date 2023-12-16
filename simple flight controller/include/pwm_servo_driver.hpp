//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_PWM_SERVO_DRIVER_H
#define BPL_PWM_SERVO_DRIVER_H

#include <cstdint>

//
// FIXME! probably does not need to have an IRQ handler
//

//#include "framework/tasking/task.hpp"
#include "framework/cli/cli_provider.hpp"

//class PWMServoDriver : public bpl::Task, public bpl::CliProvider
class PWMServoDriver : public bpl::CliProvider
{
    public:
        PWMServoDriver(const uint32_t frameRate);
        void setPWM(const uint32_t servo, const uint32_t pwmValue);

//      template <typename... int32_t>
//      void setPWMs(const int32_t... pwmValues)
//      {
//      }

//      void runTask() override;

    private:
        bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter) override;
};

#endif
