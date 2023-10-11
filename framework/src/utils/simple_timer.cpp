//
// (c) Bit Parallel Ltd, October 2023
//

#include "simple_timer.hpp"

bpl::SimpleTimer::SimpleTimer(const uint32_t period, bpl::SimpleCallBack& callBack):
    period(period), callBack(callBack) {
}

void bpl::SimpleTimer::irq()
{
    callBack();
}

uint32_t bpl::SimpleTimer::getIrqRate() const
{
    return period;
}
