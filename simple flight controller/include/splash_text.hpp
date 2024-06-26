//
// (c) Bit Parallel Ltd, May 2024
//

#ifndef BPL_SPLASH_TEXT_H
#define BPL_SPLASH_TEXT_H

#include "framework/io/text_io.hpp"

class SplashText
{
    private:
        static inline const char* splash[] = {
            R"(  ______                               .--.)",
            R"( |  ___ `.  .--.         .--.          |  |)",
            R"( ' |   `\ \ |__|  .--./) |__|     _.._ |  |.-.        .-)",
            R"( | |     \ |.--. /.''\\  .--.   .' .._||  | \ \      / /)",
            R"( | |     | ||  || |  | | |  |   | '    |  |  \ \    / /)",
            R"( | |     | ||  | \`-' /  |  | __| |__  |  |   \ \  / /)",
            R"( | |     | ||  | /("'`   |  ||__   __| |  |    \ `/ /)",
            R"( | |___./ / |  | \ '--.  |  |   | |    |  |     \  /)",
            R"( '______.'  |__|  /'"'.\ |__|   | |    |  |     / /)",
            R"(                 ||    ||       | |    '--' |`-' /)",
            R"(                 \'.__//        |_|          '..')",
            R"(                  `'--')"
        };

    public:
        static void display(const bpl::TextIO& io);
};

#endif
