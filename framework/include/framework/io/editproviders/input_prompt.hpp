//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_INPUT_PROMPT_H
#define BPL_INPUT_PROMPT_H

#include "framework/io/output_stream.hpp"
#include "framework/io/print_writer.hpp"

namespace bpl
{
    class InputPrompt
    {
        private:
            char prompt[16];
            const size_t length;

        public:
            InputPrompt();
            InputPrompt(const char* prompt);
            void display(const bpl::OutputStream& outputStream) const;
            void display(const bpl::PrintWriter& printWriter) const;
    };
}

#endif
