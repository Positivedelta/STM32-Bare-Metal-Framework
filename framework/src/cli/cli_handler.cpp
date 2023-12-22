//
// (c) Bit Parallel Ltd, October 2023
//

#include <string>
#include <utility>
#include <vector>

#include "framework/cli/cli_handler.hpp"
#include "framework/utils/string_utils.hpp"

// FIXME! for testing for char* version of bpl::TextReader::readln()
//
//#include "framework/io/editproviders/char_array_edit_buffer.hpp"

bpl::CliHandler::CliHandler(const bpl::TextIO& console, CliProviderList&& cliProviderList):
    console(console), consoleReader(console.getTextReader()), consoleWriter(console.getPrintWriter()),
    prompt(bpl::InputPrompt(PROMPT_TEXT)), history(bpl::StringEditBufferWithHistory(HISTORY_SIZE)) {
        providers = std::move(cliProviderList);

        // always initialise with a newline before printing the first ever prompt in run()
        //
        consoleWriter.println();
}

void bpl::CliHandler::run()
{
    prompt.display(consoleWriter);
    auto command = consoleReader.readln(history, prompt);

/*  // FIXME! for testing for char* version of bpl::TextReader::readln()
    //
    // note, specify (1 + buffer) size if you want to discount the 0x00 termination character
    //
    auto editBuffer = bpl::CharArrayEditBuffer<65>();
    consoleReader.readln<65>(editBuffer);
    auto command = std::pmr::string(editBuffer.buffer()); */

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
            const auto handled = provider.get().execute(tokens, console);
            if (handled) success = true;
        }

        if (!success) consoleWriter.println("Invalid command");
    }
}
