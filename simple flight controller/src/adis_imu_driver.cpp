//
// (c) Bit Parallel Ltd, October 2023
//

#include "adis_imu_driver.hpp"

AdisImuDriver::AdisImuDriver(const uint32_t period, const char* taskName, [[maybe_unused]] const LedDriver& led):
    Task(period, taskName), CliProvider("gyros", "display [#repeat] | reset") {
}

void AdisImuDriver::sample()
{
}

const AxesValues AdisImuDriver::getAxes()
{
    return AxesValues(0, 1, 2);
}

//
// protected Task method
//

void AdisImuDriver::runTask()
{
}

//
// protected CliProvider method
//

bool AdisImuDriver::handleCliCommand([[maybe_unused]] std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, [[maybe_unused]] driver::Time& time)
{
    const auto& consoleWriter = console.getPrintWriter();

    // FIXME! parse command and display the results
    //
    consoleWriter.println("The IMU command needs implementing...");
    return true;
}
