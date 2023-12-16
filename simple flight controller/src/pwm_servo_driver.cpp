//
// (c) Bit Parallel Ltd, October 2023
//

#include "pwm_servo_driver.hpp"

PWMServoDriver::PWMServoDriver(const uint32_t frameRate):
    CliProvider("servos", "manual | set #n=#pwm | auto") {
}

void PWMServoDriver::setPWM(const uint32_t servo, const uint32_t pwmValue)
{
}

//void PWMServoDriver::irq()
//{
//}

bool PWMServoDriver::handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter)
{
    // FIXME! parse command and display the results
    //
    consoleWriter.println("The SERVOS command needs implementing...");
    return true;
}
