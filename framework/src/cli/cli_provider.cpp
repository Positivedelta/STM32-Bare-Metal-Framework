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
bool bpl::CliProvider::execute(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console)
{
    if (commandTokens.front() == "help")
    {
        const auto& consoleWriter = console.getPrintWriter();

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

    // not a help command, so delegate to the appropriate handler
    //
    if (commandTokens.front() == getCommandName())
    {
        // notes 1, the handler can reject the command, bad syntax, invalid value etc, ...
        //       2, under these circumstances, it's expected that the other handlers will return false (as they are different commands)
        //          and the CliHandler instance will report an "Invalid command" (see below)
        //
        return handleCliCommand(commandTokens, console);
    }
    else
    {
        // notes 1, no handler found for this command
        //       2, the CliHandler instance will try the other registered handlers
        //       3, if the command remains unhandled, the CliHandler instance will report an "Invalid command"
        //
        return false;
    }
}

const std::pmr::string& bpl::CliProvider::getCommandName() const
{
    return name;
}

const std::pmr::string& bpl::CliProvider::getCommandHelp() const
{
    return help;
}
