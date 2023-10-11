//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_SBUS_DRIVER_H
#define BPL_SBUS_DRIVER_H

//
// FIXME! probably does not need to have an IRQ handler
//

//#include "timer_irq_consumer.hpp"
#include "cli_provider.hpp"
//#include "sbus_values.hpp"

//class SBusDriver : public bpl::TimerIrqConsumer, public CliProvider
class SBusDriver : public CliProvider
{
    public:
        SBusDriver();
//      const SBusValues getSBusValues();

//      void irq() override;

    protected:
        bool doExecute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter) override;
};

#endif
