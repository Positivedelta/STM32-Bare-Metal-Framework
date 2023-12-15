//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_CLI_PROVIDER_LIST_H
#define BPL_CLI_PROVIDER_LIST_H

#include <functional>
#include <utility>
#include <vector>

#include "framework/cli/cli_provider.hpp"

namespace bpl
{
    class CliProviderList : public std::pmr::vector<std::reference_wrapper<bpl::CliProvider>>
    {
        public:
            template <typename ...T>
            CliProviderList(T&& ...t) : std::pmr::vector<std::reference_wrapper<bpl::CliProvider>>{{std::forward<T>(t)...}}
            {
            }
    };
}

#endif
