//
// (c) Bit Parallel Ltd, October 2023
//

#include "pinpoint_gyro_driver.hpp"

PinPointGyroDriver::PinPointGyroDriver(const uint32_t period, const char* taskName, const LedDriver& led):
    Task(period, taskName), CliProvider("gyros", "display [#repeat] | reset") {
}

void PinPointGyroDriver::sample()
{
}

const AxesValues PinPointGyroDriver::getAxes()
{
    return AxesValues(0, 1, 2);
}

//
// protected Task method
//

void PinPointGyroDriver::runTask()
{
}

//
// protected CliProvider method
//

bool PinPointGyroDriver::handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, driver::Time& time)
{
    const auto& consoleWriter = console.getPrintWriter();

    // FIXME! parse command and display the results
    //
    consoleWriter.println("The GYROS command needs implementing...");
    return true;
}
