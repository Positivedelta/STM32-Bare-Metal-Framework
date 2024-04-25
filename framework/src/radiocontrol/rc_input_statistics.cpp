//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/radiocontrol/rc_input_statistics.hpp"

bpl::RcInputStatistics::RcInputStatistics():
    newDataCount(0), staleDataCount(0), failsafeConditionCount(0), packetErrorCount(0), crcErrorCount(0), parityErrorCount(0) {
}

void bpl::RcInputStatistics::incrementNewDataCount()
{
    newDataCount++;
}

// don't start accumulating this statistic until at least one good RC update has been received
//
void bpl::RcInputStatistics::incrementStaleDataCount()
{
    if (newDataCount > 0) staleDataCount++;
}

void bpl::RcInputStatistics::incrementFailsafeConditionCount()
{
    failsafeConditionCount++;
}

void bpl::RcInputStatistics::incrementPacketErrorCount()
{
    packetErrorCount++;
}

void bpl::RcInputStatistics::incrementCrcErrorCount()
{
    crcErrorCount++;
}

void bpl::RcInputStatistics::incrementParityErrorCount()
{
    parityErrorCount++;
}

uint32_t bpl::RcInputStatistics::getNewDataCount() const
{
    return newDataCount;
}

uint32_t bpl::RcInputStatistics::getStaleDataCount() const
{
    return staleDataCount;
}

uint32_t bpl::RcInputStatistics::getFailsafeConditionCount() const
{
    return failsafeConditionCount;
}

uint32_t bpl::RcInputStatistics::getPacketErrorCount() const
{
    return packetErrorCount;
}

uint32_t bpl::RcInputStatistics::getCrcErrorCount() const
{
    return crcErrorCount;
}

uint32_t bpl::RcInputStatistics::getParityErrorCount() const
{
    return parityErrorCount;
}
