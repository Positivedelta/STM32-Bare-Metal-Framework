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
#include "framework/io/editproviders/input_prompt.hpp"
#include "framework/io/editproviders/string_edit_buffer_with_history.hpp"

// FIXME! for testing for char* version of bpl::TextReader::readln()
//
//#include "framework/io/editproviders/char_array_edit_buffer_with_history.hpp"

namespace bpl
{
    class CliHandler
    {
        using WrappedCliProvider = std::reference_wrapper<bpl::CliProvider>;
        using CliProviders = std::pmr::vector<WrappedCliProvider>;

        public:
            static constexpr char const* PROMPT_TEXT = "# ";
            static constexpr size_t HISTORY_SIZE = 8;

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
            bpl::StringEditBufferWithHistory history;

//          // FIXME! for testing for char* version of bpl::TextReader::readln()
//          //
//          bpl::CharArrayEditBufferWithHistory<65, 8> history;

        // note, bpl::TextIO will accept implementations of bpl::IOStream, e.g. uarts
        //
        public:
            CliHandler(const bpl::TextIO& console, CliProviderList&& cliProviderList);
            void run();
    };
}

#endif
