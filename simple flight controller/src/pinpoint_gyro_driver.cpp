//
// (c) Bit Parallel Ltd, October 2023
//

#include "pinpoint_gyro_driver.hpp"

PinPointGyroDriver::PinPointGyroDriver(const uint32_t samplingInterval, const LedDriver& led):
    CliProvider("gyros", "[repeat] display | reset"), samplingInterval(samplingInterval) {
}

void PinPointGyroDriver::sample()
{
}

const AxesValues PinPointGyroDriver::getAxes()
{
    return AxesValues(0, 1, 2);
}

void PinPointGyroDriver::irq()
{
}

uint32_t PinPointGyroDriver::getIrqRate() const
{
    return samplingInterval;
}

bool PinPointGyroDriver::doExecute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter)
{
    if (commandTokens.front() == getName())
    {
        // FIXME! parse command and display the results
        //
        consoleWriter.println("The GYROS command needs implementing...");
        return true;
    }

    return false;
}
