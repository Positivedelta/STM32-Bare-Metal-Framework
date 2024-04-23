//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_RC_INPUT_H
#define BPL_RC_INPUT_H

#include <cstdint>

#include "framework/cli/cli_provider.hpp"
#include "framework/drivers/time.hpp"
#include "framework/drivers/Uart.hpp"
#include "framework/radiocontrol/rc_decoder.hpp"


// FIXME! probably does not need to have an IRQ handler
//
//#include "framework/irq/timer_irq_consumer.hpp"

//
// FIXME! must implement...
//        1, cli interface
//        2, channel mapping
//        3, flight mode
//

namespace bpl
{
    //class RcInputDriver : public bpl::TimerIrqConsumer, public CliProvider
    class RcInput : public bpl::CliProvider
    {
        public:
            static constexpr int32_t ABSOLUTE_MAX_CHANNEL_SHIFT = 12;
            static constexpr int32_t ABSOLUTE_MAX_CHANNEL_VALUE = 1 << 12;

        private:
            bpl::RcDecoder& rcDecoder;
            bool useCyclicRing;
            int32_t alieron, elevator;

        public:
            RcInput(bpl::RcDecoder& rcDecoder);

            void setCyclicRing(const bool enable = true);
            bool hasCyclicRing() const;

            // note, the RC channels are values in the range [-4096..4096] or [0..4096], as defined in the header file
            //
            void aquire();
            uint32_t getThrottle() const;        // returns values in the range [0..4096]
            int32_t getAileron() const;
            int32_t getElevator() const;
            int32_t getRudder() const;
            int32_t getPitch() const;
//          bpl::FlightMode getFlightMode() const;

            // FIXME! these are not generic!
            //
//          bpl::RcStatus getLastStatus();
//          bpl::RcStatistics getStatistics();

    //      void irq() override;

        private:
            bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console) override;
    };
}

#endif
