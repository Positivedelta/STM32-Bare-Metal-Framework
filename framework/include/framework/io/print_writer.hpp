//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_PRINT_WRITER_H
#define BPL_PRINT_WRITER_H

#include <cstdint>
#include <string_view>

#include "framework/io/output_stream.hpp"

namespace bpl
{
    class PrintWriter
    {
        private:
            const bpl::OutputStream& outputStream;

        public:
            PrintWriter(const bpl::OutputStream& outputStream);

            // notes 1, the print() & println() methods take std::string_view or uint8_t* arguments
            //       2, the use of std::prm::string will incur PMR memory allocation
            //
            const bool print(const uint8_t byte) const;
            const uint8_t print(const char* text) const;
            const uint8_t print(const char* text, const size_t length) const;
            const uint8_t print(const std::string_view& text) const;
            const uint8_t println(const char* text) const;
            const uint8_t println(const std::string_view& text) const;
            const uint8_t println() const;
    };
}

#endif
