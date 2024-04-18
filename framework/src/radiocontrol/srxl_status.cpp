//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/radiocontrol/srxl_status.hpp"

bpl::SrxlStatus::SrxlStatus():
    newData(false), staleData(false), headerError(false), crcError(false),
    newDataCount(0), staleDataCount(0), headerErrorCount(0), crcErrorCount(0) {
}

void bpl::SrxlStatus::clearFlags()
{
    newData = false;
    staleData = false;
    headerError = false;
    crcError = false;
}

void bpl::SrxlStatus::indicateNewData()
{
    newData = true;
    newDataCount++;
}

void bpl::SrxlStatus::indicateStaleData()
{
    staleData = true;
    staleDataCount++;
}

void bpl::SrxlStatus::indicateHeaderError()
{
    headerError = true;
    headerErrorCount++;
}

void bpl::SrxlStatus::indicateCrcError()
{
    crcError = true;
    crcErrorCount++;
}

bool bpl::SrxlStatus::hasNewData() const
{
    return newData;
}

bool bpl::SrxlStatus::hasStaleData() const
{
    return staleData;
}

bool bpl::SrxlStatus::hasHeaderError() const
{
    return headerError;
}

bool bpl::SrxlStatus::hasCrcError() const
{
    return crcError;
}

uint32_t bpl::SrxlStatus::getNewDataCount() const
{
    return newDataCount;
}

uint32_t bpl::SrxlStatus::getStaleDataCount() const
{
    return staleDataCount;
}

uint32_t bpl::SrxlStatus::getHeaderErrorCount() const
{
    return headerErrorCount;
}

uint32_t bpl::SrxlStatus::getCrcErrorCount() const
{
    return crcErrorCount;
}
