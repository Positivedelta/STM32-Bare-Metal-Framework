//
// (c) Bit Parallel Ltd, May 2024
//

#include "splash_text.hpp"

void SplashText::display(const bpl::TextIO& io)
{
    const auto& writer = io.getPrintWriter();
    writer.println();
    for (auto& line : splash) writer.println(line);
}
