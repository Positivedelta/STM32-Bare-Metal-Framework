//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/radiocontrol/srxl_status.hpp"

bpl::SrxlStatus::SrxlStatus():
    newData(false), staleData(false), failsafeCondition(false), packetHeaderError(false), packetTypeError(false), crcError(false) {
}

void bpl::SrxlStatus::clearFlags()
{
    newData = false;
    staleData = false;
    failsafeCondition = false;
    packetHeaderError = false;
    packetTypeError = false;
    crcError = false;
}

void bpl::SrxlStatus::indicateNewData()
{
    newData = true;
}

void bpl::SrxlStatus::indicateStaleData()
{
    staleData = true;
}

void bpl::SrxlStatus::indicateFailsafeCondition()
{
    failsafeCondition = true;
}

void bpl::SrxlStatus::indicatePacketHeaderError()
{
    packetHeaderError = true;
}

void bpl::SrxlStatus::indicatePacketTypeError()
{
    packetTypeError = true;
}

void bpl::SrxlStatus::indicateCrcError()
{
    crcError = true;
}

bool bpl::SrxlStatus::hasNewData() const
{
    return newData;
}

bool bpl::SrxlStatus::hasStaleData() const
{
    return staleData;
}

bool bpl::SrxlStatus::hasFailsafeCondition() const
{
    return failsafeCondition;
}

bool bpl::SrxlStatus::hasPacketHeaderError() const
{
    return packetHeaderError;
}

bool bpl::SrxlStatus::hasPacketTypeError() const
{
    return packetTypeError;
}

bool bpl::SrxlStatus::hasCrcError() const
{
    return crcError;
}
