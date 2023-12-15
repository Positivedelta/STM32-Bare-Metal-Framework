//
// (c) Bit Parallel Ltd, October 2023
//

#include "sbus_driver.hpp"

SBusDriver::SBusDriver():
    CliProvider("sbus", "[repeat] all | #ch | #ch #ch ..#ch | #ch:#ch") {
}

//const SBusValues getSBusValues()
//{
//}

//void SBusDriver::irq()
//{
//}

bool SBusDriver::handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter)
{
    if (commandTokens.front() == getCommandName())
    {
        // FIXME! parse the sbus command and display the results
        //
        consoleWriter.println("The SBUS command needs implementing...");
        return true;
    }

    return false;
}
