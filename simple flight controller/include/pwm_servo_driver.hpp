//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_PWM_SERVO_DRIVER_H
#define BPL_PWM_SERVO_DRIVER_H

#include <cstdint>

//
// FIXME! probably does not need to have an IRQ handler
//

//#include "irq_consumer.hpp"
#include "cli_provider.hpp"

//class PWMServoDriver : public bpl::IrqConsumer, public CliProvider
class PWMServoDriver : public CliProvider
{
    public:
        PWMServoDriver(const uint32_t frameRate);
        void setPWM(const uint32_t servo, const uint32_t pwmValue);

//      template <typename... int32_t>
//      void setPWMs(const int32_t... pwmValues)
//      {
//      }

//      void irq() override;

    protected:
        bool doExecute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter) override;
};

#endif
