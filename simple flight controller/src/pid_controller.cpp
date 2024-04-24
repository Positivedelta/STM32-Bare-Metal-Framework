//
// (c) Bit Parallel Ltd, October 2023
//

#include "pid_controller.hpp"

PidController::PidController(const uint32_t period, const char* taskName, const PinPointGyroDriver& gyros, bpl::RcInput& rcInput, const PWMServoDriver& servos, const LedDriver& led):
    Task(period, taskName), CliProvider("loop", "set P=#gain and/or I=#gain and/or D=#gain"),
    rcInput(rcInput) {
}

//
// protected Task method
//

void PidController::runTask()
{
    rcInput.setCyclicRing();
    rcInput.aquire();
}

//
// protected CliProvider method
//

bool PidController::handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console)
{
    const auto& consoleWriter = console.getPrintWriter();

    // FIXME! parse command and display the results
    //
    consoleWriter.println("The LOOP command needs implementing...");
    return true;
}
