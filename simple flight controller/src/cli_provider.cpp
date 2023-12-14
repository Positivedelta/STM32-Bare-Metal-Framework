//
// (c) Bit Parallel Ltd, October 2023
//

#include "framework/utils/string_utils.hpp"
#include "cli_provider.hpp"

CliProvider::CliProvider(std::pmr::string name, std::pmr::string help):
    name(name), help(help) {
}

// notes 1, the help commands are dealt with generically by this method
//       2, all other commands are delegated to the inheriting instances and are then handled if appropriate by the given instances
//       3, only a single instance is expected to handle a specific non help command
//
bool CliProvider::execute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter)
{
    if (commandTokens.front() == "help")
    {
        const auto parts = commandTokens.size();
        if (parts == 1)
        {
            consoleWriter.println(getName());
            return true;
        }

        // only expecting 2 parts, anything else is invalid
        //
        if ((parts == 2) && ((commandTokens.back() == getName()) || (commandTokens.back() == "all")))
        {
            consoleWriter.println(getName() + " " + getHelp());
            return true;
        }

        return false;
    }

    // not a help command, so delegate
    //
    return doExecute(commandTokens, consoleWriter);
}

const std::pmr::string& CliProvider::getName() const
{
    return name;
}

const std::pmr::string& CliProvider::getHelp() const
{
    return help;
}
