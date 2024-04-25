//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_CLI_PROVIDER_H
#define BPL_CLI_PROVIDER_H

#include <cstdint>
#include <memory_resource>
#include <string>
#include <vector>

#include "framework/drivers/time.hpp"
#include "framework/io/text_io.hpp"

namespace bpl
{
    class CliProvider
    {
        protected:
            const std::pmr::string name;
            const std::pmr::string help;

        protected:
            CliProvider(std::pmr::string name, std::pmr::string help);

            // notes 1, expected to be called by execute() for non help commands
            //       2, the time reference is useful when implementing commands that repeat / refresh at a user defined rate
            //
            virtual bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, driver::Time& time) = 0;

        public:
            const std::pmr::string& getCommandName() const;
            const std::pmr::string& getCommandHelp() const;

            // notes 1, the help commands are dealt with by this method
            //       2, all other commands are delegated to the inheriting instances and are must be handled appropriately by handleCliCommand()
            //       3, only a single instance is expected to handle a specific non help command
            //       4, the time reference is useful when implementing commands that repeat / refresh at a user defined rate
            //
            bool execute(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, driver::Time& time);
    };
}

#endif
