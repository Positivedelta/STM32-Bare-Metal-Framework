//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_CLI_PROVIDER_H
#define BPL_CLI_PROVIDER_H

#include <cstdint>
#include <memory_resource>
#include <string>
#include <vector>

#include "print_writer.hpp"

class CliProvider
{
    protected:
        const std::pmr::string name;
        const std::pmr::string help;

    protected:
        CliProvider(std::pmr::string name, std::pmr::string help);

        // note, expected to be called by execute() for non help commands
        //
        virtual bool doExecute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter) = 0;

    public:
        const std::pmr::string& getName() const;
        const std::pmr::string& getHelp() const;

        // notes 1, the help commands are dealt with generically by this method
        //       2, all other commands are delegated to the inheriting instances and are then handled by doExecute(), if appropriate by the given instances
        //       3, only a single instance is expected to handle a specific non help command
        //
        bool execute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter);
};

#endif
