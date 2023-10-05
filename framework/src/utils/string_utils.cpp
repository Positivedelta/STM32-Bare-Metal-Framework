//
// (c) Bit Parallel Ltd, October 2023
//

#include <ranges>

#include "string_utils.hpp"

void bpl::StringUtils::trim(std::pmr::string& s)
{
    auto notSpace = [](unsigned char c) {
        return !std::isspace(c);
    };

    // erase the the spaces at the back first, prevents extra work, then from the front
    //
    s.erase(std::ranges::find_if(s | std::views::reverse, notSpace).base(), s.end());
    s.erase(s.begin(), std::ranges::find_if(s, notSpace));
}
