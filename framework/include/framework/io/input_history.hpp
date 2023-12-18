//
// (c) Bit Parallel Ltd, December 2023
//

#ifndef BPL_INPUT_HISTORY_H
#define BPL_INPUT_HISTORY_H

namespace bpl
{
    template<typename T>
    class InputHistory
    {
        public:
            virtual const bool back() = 0;
            virtual const bool forward() = 0;
            virtual T& input() = 0;
    };
}

#endif
