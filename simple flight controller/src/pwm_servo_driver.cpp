//
// (c) Bit Parallel Ltd, October 2023
//

#include "pwm_servo_driver.hpp"

PWMServoDriver::PWMServoDriver([[maybe_unused]] const uint32_t frameRate):
    CliProvider("servos", "manual | set #n=#pwm | auto") {
}

void PWMServoDriver::setPWM([[maybe_unused]] const uint32_t servo, [[maybe_unused]] const uint32_t pwmValue)
{
}

//void PWMServoDriver::irq()
//{
//}

bool PWMServoDriver::handleCliCommand([[maybe_unused]] std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, [[maybe_unused]] driver::Time& time)
{
    const auto& consoleWriter = console.getPrintWriter();

    // FIXME! parse command and display the results
    //
    consoleWriter.println("The SERVOS command needs implementing...");
    return true;
}
