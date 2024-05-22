//
// (c) Bit Parallel Ltd, October 2023
//

#include "stability_controller.hpp"

StabilityController::StabilityController(const uint32_t period, const char* taskName, const AdisImuDriver& imu, bpl::RcInput& rcInput, const PWMServoDriver& servos, const LedDriver& led):
    Task(period, taskName), CliProvider("imu", "set P=#gain and/or I=#gain and/or D=#gain"),
    rcInput(rcInput) {
}

//
// protected Task method
//

void StabilityController::runTask()
{
    rcInput.setCyclicRing();
    rcInput.aquire();
}

//
// protected CliProvider method
//

bool StabilityController::handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, driver::Time& time)
{
    const auto& consoleWriter = console.getPrintWriter();

    // FIXME! parse command and display the results
    //
    consoleWriter.println("The IMU command needs implementing...");
    return true;
}
