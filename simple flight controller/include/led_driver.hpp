//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_LED_DRIVER_H
#define BPL_LED_DRIVER_H

#include <cstdint>

#include "framework/tasking/task.hpp"
#include "cli_provider.hpp"

class LedDriver : public bpl::Task, public CliProvider
{
    private:
        volatile int32_t ledPeriod, ledPeriodCount;
        volatile bool isActive, ledOn, ledFlashing, ledToggle;
        int32_t currentLedPeriod, currentLedPeriodCount;
        bool currentLedOn, currentLedFlashing, currentLedToggle;

    public:
        LedDriver(const uint32_t period, const char* taskName);

        void on();
        void off();
        void flash(const uint32_t period);

    protected:
        void runTask() override;
        bool doExecute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter) override;

    private:
        void doLedOn();
        void doLedOff();
        void doLedFlash(const uint32_t period);

        void ifActiveSaveStateAndReportCliControl(const bpl::PrintWriter& consoleWriter);
        void restoreActiveState();
};

#endif
