//
// (c) Bit Parallel Ltd, October 2023
//

#include <algorithm>
#include <ranges>

#include "string_utils.hpp"

void bpl::StringUtils::trim(std::pmr::string& string)
{
    auto notSpace = [](const unsigned char c) {
        return !std::isspace(c);
    };

    // erase the the spaces at the back first, prevents extra work, then from the front
    //
    string.erase(std::ranges::find_if(string | std::views::reverse, notSpace).base(), string.end());
    string.erase(string.begin(), std::ranges::find_if(string, notSpace));
}

void bpl::StringUtils::strip(std::pmr::string& string, const unsigned char c)
{
    auto isSpace = [](unsigned char const c) {
        return std::isspace(c);
    };

    string.erase(std::remove_if(string.begin(), string.end(), isSpace), string.end());
}

void bpl::StringUtils::tokenize(const std::string_view& stringView, const char separator, std::pmr::vector<std::string_view>& tokens)
{
    auto split = stringView | std::ranges::views::split(separator);
    for (const auto& token : split)
    {
        if (token.begin() == token.end()) continue;
        tokens.emplace_back(std::string_view(token.begin(), token.end()));
    }
}
