//
// (c) Bit Parallel Ltd, October 2023
//

#include <string>
#include <utility>
#include <vector>

#include "framework/cli/cli_handler.hpp"
#include "framework/utils/string_utils.hpp"

bpl::CliHandler::CliHandler(const bpl::TextIO& console, CliProviderList&& cliProviderList, driver::Time& time):
    console(console), time(time), consoleReader(console.getTextReader()), consoleWriter(console.getPrintWriter()),
    prompt(bpl::InputPrompt(PROMPT_TEXT)), history(bpl::StringEditBufferWithHistory(HISTORY_SIZE)) {
//  prompt(bpl::InputPrompt(PROMPT_TEXT)), history(bpl::CharArrayEditBufferWithHistory<65, 8>()) {      // FIXME! for testing the char* version of bpl::TextReader::readln()
        providers = std::move(cliProviderList);

        // always initialise with a newline before printing the first ever prompt in run()
        //
        consoleWriter.println();
}

void bpl::CliHandler::run()
{
    prompt.display(consoleWriter);
    auto command = consoleReader.readln(history, prompt);

//  // FIXME! for testing for char* version of bpl::TextReader::readln()
//  //
//  // note, specify (1 + buffer) size if you want to discount the 0x00 termination character
//  //
//  consoleReader.readln<65>(history, prompt);
//  auto command = std::pmr::string(history.buffer());

    // note, the tokenize() method also trims by definition when using ' ' tokens
    //
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
            const auto handled = provider.get().execute(tokens, console, time);
            if (handled) success = true;
        }

        if (!success) consoleWriter.println("Invalid command");
    }
}
