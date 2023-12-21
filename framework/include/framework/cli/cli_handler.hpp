//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_CLI_HANDLER_H
#define BPL_CLI_HANDLER_H

#include <functional>
#include <memory_resource>
#include <string>
#include <vector>

#include "framework/cli/cli_provider.hpp"
#include "framework/io/text_reader.hpp"
#include "framework/io/print_writer.hpp"
#include "framework/io/text_io.hpp"
#include "framework/io/input_prompt.hpp"
#include "framework/io/string_input_history.hpp"

namespace bpl
{
    class CliHandler
    {
        using WrappedCliProvider = std::reference_wrapper<bpl::CliProvider>;
        using CliProviders = std::pmr::vector<WrappedCliProvider>;

        private:
            class CliProviderList : public CliProviders
            {
                public:
                    template <typename ...T>
                    CliProviderList(T&& ...t):
                        std::pmr::vector<WrappedCliProvider>{std::forward<T>(t)...} {
                    }
            };

            CliProviderList providers;
            const bpl::TextIO& console;
            const bpl::TextReader& consoleReader;
            const bpl::PrintWriter& consoleWriter;
            const bpl::InputPrompt prompt;
            bpl::StringInputHistory history;

        // note, bpl::TextIO will accept implementations of bpl::IOStream, e.g. uarts
        //
        public:
            CliHandler(const bpl::TextIO& console, CliProviderList&& cliProviderList);
            void run();
    };
}

#endif
