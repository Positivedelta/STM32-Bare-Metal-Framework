//
// (c) Bit Parallel Ltd, October 2023
//

#include "framework/cli/cli_provider.hpp"
#include "framework/utils/string_utils.hpp"

bpl::CliProvider::CliProvider(std::pmr::string name, std::pmr::string help):
    name(name), help(help) {
}

// notes 1, the help commands are dealt with generically by this method
//       2, all other commands are delegated to the inheriting instances and are then handled if appropriate by the given instances
//       3, only a single instance is expected to handle a specific non help command
//
bool bpl::CliProvider::execute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter)
{
    if (commandTokens.front() == "help")
    {
        const auto parts = commandTokens.size();
        if (parts == 1)
        {
            consoleWriter.println(getCommandName());
            return true;
        }

        // only expecting 2 parts, anything else is invalid
        //
        if ((parts == 2) && ((commandTokens.back() == getCommandName()) || (commandTokens.back() == "all")))
        {
            consoleWriter.println(getCommandName() + " " + getCommandHelp());
            return true;
        }

        return false;
    }

    // not a help command, so delegate
    //
    return handleCliCommand(commandTokens, consoleWriter);
}

const std::pmr::string& bpl::CliProvider::getCommandName() const
{
    return name;
}

const std::pmr::string& bpl::CliProvider::getCommandHelp() const
{
    return help;
}
