//
// (c) Bit Parallel Ltd, October 2023
//

#include "pid_controller.hpp"

PidController::PidController(const uint32_t loopRate, const PinPointGyroDriver& gyros, const SBusDriver& sbus, const PWMServoDriver& servos, const LedDriver& led):
    CliProvider("loop", "set P=#gain and/or I=#gain and/or D=#gain"), loopRate(loopRate) {
}

void PidController::run()
{
}

void PidController::irq()
{
}

uint32_t PidController::getIrqRate() const
{
    return loopRate;
}

bool PidController::doExecute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter)
{
    if (commandTokens.front() == getName())
    {
        // FIXME! parse command and display the results
        //
        consoleWriter.println("The LOOP command needs implementing...");
        return true;
    }

    return false;
}
