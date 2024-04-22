//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/radiocontrol/srxl_statistics.hpp"

bpl::SrxlStatistics::SrxlStatistics():
    newDataCount(0), staleDataCount(0), failsafeConditionCount(0), packetHeaderErrorCount(0), packetTypeErrorCount(0), crcErrorCount(0) {
}

void bpl::SrxlStatistics::incrementNewDataCount()
{
    newDataCount++;
}

void bpl::SrxlStatistics::incrementStaleDataCount()
{
    staleDataCount++;
}

void bpl::SrxlStatistics::incrementFailsafeConditionCount()
{
    failsafeConditionCount++;
}

void bpl::SrxlStatistics::incrementPacketHeaderErrorCount()
{
    packetHeaderErrorCount++;
}

void bpl::SrxlStatistics::incrementPacketTypeErrorCount()
{
    packetTypeErrorCount++;
}

void bpl::SrxlStatistics::incrementCrcErrorCount()
{
    crcErrorCount++;
}

uint32_t bpl::SrxlStatistics::getNewDataCount() const
{
    return newDataCount;
}

uint32_t bpl::SrxlStatistics::getStaleDataCount() const
{
    return staleDataCount;
}

uint32_t bpl::SrxlStatistics::getFailsafeConditionCount() const
{
    return failsafeConditionCount;
}

uint32_t bpl::SrxlStatistics::getPacketHeaderErrorCount() const
{
    return packetHeaderErrorCount;
}

uint32_t bpl::SrxlStatistics::getPacketTypeErrorCount() const
{
    return packetTypeErrorCount;
}

uint32_t bpl::SrxlStatistics::getCrcErrorCount() const
{
    return crcErrorCount;
}
