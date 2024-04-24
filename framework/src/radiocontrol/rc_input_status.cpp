//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/radiocontrol/rc_input_status.hpp"

bpl::RcInputStatus::RcInputStatus():
    newData(false), staleData(false), failsafeCondition(false), packetError(false), crcError(false), parityError(false) {
}

void bpl::RcInputStatus::clearFlags()
{
    newData = false;
    staleData = false;
    failsafeCondition = false;
    packetError = false;
    crcError = false;
    parityError = false;
}

void bpl::RcInputStatus::indicateNewData()
{
    newData = true;
}

void bpl::RcInputStatus::indicateStaleData()
{
    staleData = true;
}

void bpl::RcInputStatus::indicateFailsafeCondition()
{
    failsafeCondition = true;
}

void bpl::RcInputStatus::indicatePacketError()
{
    packetError = true;
}

void bpl::RcInputStatus::indicateCrcError()
{
    crcError = true;
}

void bpl::RcInputStatus::indicateParityError()
{
    parityError = true;
}

bool bpl::RcInputStatus::hasNewData() const
{
    return newData;
}

bool bpl::RcInputStatus::hasStaleData() const
{
    return staleData;
}

bool bpl::RcInputStatus::hasFailsafeCondition() const
{
    return failsafeCondition;
}

bool bpl::RcInputStatus::hasPacketError() const
{
    return packetError;
}

bool bpl::RcInputStatus::hasCrcError() const
{
    return crcError;
}

bool bpl::RcInputStatus::hasParityError() const
{
    return parityError;
}
