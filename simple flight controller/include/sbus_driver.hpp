//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_SBUS_DRIVER_H
#define BPL_SBUS_DRIVER_H

//
// FIXME! probably does not need to have an IRQ handler
//

//#include "framework/irq/timer_irq_consumer.hpp"
#include "framework/cli/cli_provider.hpp"
//#include "sbus_values.hpp"

//class SBusDriver : public bpl::TimerIrqConsumer, public CliProvider
class SBusDriver : public bpl::CliProvider
{
    public:
        SBusDriver();
//      const SBusValues getSBusValues();

//      void irq() override;

    protected:
        bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter) override;
};

#endif
