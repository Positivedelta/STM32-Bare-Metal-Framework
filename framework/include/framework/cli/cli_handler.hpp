//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_CLI_HANDLER_H
#define BPL_CLI_HANDLER_H

#include "framework/cli/cli_provider_list.hpp"
#include "framework/io/text_reader.hpp"
#include "framework/io/print_writer.hpp"
#include "framework/io/text_io.hpp"

namespace bpl
{
    class CliHandler
    {
        private:
            bpl::CliProviderList providers;
            const bpl::TextReader& consoleReader;
            const bpl::PrintWriter& consoleWriter;

        public:
            CliHandler(const bpl::TextIO& console, bpl::CliProviderList&& cliProviderList);
            void run();
    };
}

#endif
