//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_CLI_HANDLER_H
#define BPL_CLI_HANDLER_H

#include "text_reader.hpp"
#include "print_writer.hpp"
#include "text_io.hpp"
#include "cli_provider_list.hpp"

class CliHandler
{
    private:
        CliProviderList providers;
        const bpl::TextReader& consoleReader;
        const bpl::PrintWriter& consoleWriter;

    public:
        CliHandler(const bpl::TextIO& console, CliProviderList&& cliProviderList);
        void run();
};

#endif
