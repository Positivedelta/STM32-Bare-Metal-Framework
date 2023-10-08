//
// (c) Bit Parallel Ltd, October 2023
//

#include <string>
#include <utility>
#include <vector>

#include "string_utils.hpp"
#include "cli_handler.hpp"

CliHandler::CliHandler(const bpl::TextIO& console, CliProviderList&& cliProviderList):
    consoleReader(console.getTextReader()), consoleWriter(console.getPrintWriter()) {
        providers = std::move(cliProviderList);

        // always initialise with a prompt on a newline
        //
        consoleWriter.println();
}

void CliHandler::run()
{
    consoleWriter.print("# ");

    // note, the tokenize() method also trims by definition
    //
    auto command = consoleReader.readln();
    auto tokens = std::pmr::vector<std::string_view>();
    bpl::StringUtils::tokenize(command, ' ', tokens);
    if (tokens.size() > 0)
    {
        // special case to display "help all" as this case can't be delegated to a CliProvider instance
        //
        if ((tokens.size() == 1) && (tokens.front() == "help")) consoleWriter.println("all");

        auto success = false;
        for (auto& provider : providers)
        {
            const auto handled = provider.get().execute(tokens, consoleWriter);
            if (handled) success = true;
        }

        if (!success) consoleWriter.println("Invalid command");
    }
}
