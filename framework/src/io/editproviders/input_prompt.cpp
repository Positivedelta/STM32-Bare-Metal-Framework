//
// (c) Bit Parallel Ltd, December 2023
//

#include <cstdint>
#include <cstdio>

#include "framework/io/editproviders/input_prompt.hpp"

bpl::InputPrompt::InputPrompt():
    length(0) {
}

bpl::InputPrompt::InputPrompt(const char* inputPrompt):
    length(std::snprintf(prompt, sizeof(prompt), inputPrompt)) {
}

void bpl::InputPrompt::display(const bpl::OutputStream& outputStream) const
{
    if (length > 0) outputStream.write(reinterpret_cast<const uint8_t*>(prompt), length);
}

void bpl::InputPrompt::display(const bpl::PrintWriter& printWriter) const
{
    if (length > 0) printWriter.print(prompt, length);
}
