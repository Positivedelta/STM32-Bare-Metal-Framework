//
// (c) Bit Parallel Ltd, September 2023
//

#include "simple_timer.hpp"

bpl::SimpleTimer::SimpleTimer(const int32_t period, bpl::SimpleCallBack& callBack):
    period(period), callBack(callBack), counter(period) {
}

void bpl::SimpleTimer::irq()
{
    if (--counter > 0) return;

    callBack();
    counter = period;
}
