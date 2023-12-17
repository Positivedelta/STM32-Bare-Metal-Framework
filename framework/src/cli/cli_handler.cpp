//
// (c) Bit Parallel Ltd, October 2023
//

#include <string>
#include <utility>
#include <vector>

#include "framework/cli/cli_handler.hpp"
#include "framework/utils/string_utils.hpp"

bpl::CliHandler::CliHandler(const bpl::TextIO& console, CliProviderList&& cliProviderList):
    console(console), consoleReader(console.getTextReader()), consoleWriter(console.getPrintWriter()) {
        providers = std::move(cliProviderList);

        // always initialise with a newline before printing the first ever prompt in run()
        //
        consoleWriter.println();
}

void bpl::CliHandler::run()
{
    consoleWriter.print("# ");

    // note, the tokenize() method also trims by definition
    //
    auto command = consoleReader.readln();
    auto tokens = std::pmr::vector<std::string_view>();
    bpl::StringUtils::tokenize(command, ' ', tokens);
    if (tokens.size() > 0)
    {
        // special case to display "all" as a help option, this case can't be delegated to the individual CliProvider instances
        //
        if ((tokens.size() == 1) && (tokens.front() == "help")) consoleWriter.println("all");

        auto success = false;
        for (auto& provider : providers)
        {
            const auto handled = provider.get().execute(tokens, console);
            if (handled) success = true;
        }

        if (!success) consoleWriter.println("Invalid command");
    }
}
