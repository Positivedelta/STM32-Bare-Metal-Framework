//
// (c) Bit Parallel Ltd, April 2024
//

#ifndef BPL_RC_INPUT_H
#define BPL_RC_INPUT_H

#include <cstdint>

#include "framework/cli/cli_provider.hpp"
#include "framework/drivers/Uart.hpp"
#include "framework/radiocontrol/rc_decoder.hpp"

//
// FIXME! must implement...
//        1, channel mapping
//        2, flight mode
//

namespace bpl
{
    class RcInput : public bpl::CliProvider
    {
        public:
            constexpr static uint32_t MAX_NUMBER_OF_CHANNELS = 32;
            constexpr static int32_t ABSOLUTE_MAX_CHANNEL_SHIFT = 12;
            constexpr static int32_t ABSOLUTE_MAX_CHANNEL_VALUE = 1 << 12;

        private:
            bpl::RcDecoder& rcDecoder;
            bool useCyclicRing;
            int32_t aileron, elevator;
            char cliReceiverString[32];

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

            bpl::RcInputStatus getStatus() const;
            bpl::RcInputStatistics getStatistics() const;

        private:
            bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, driver::Time& time) override;
            void printChannelValues(const bpl::PrintWriter& consoleWriter);
            void printChannelStatistics(const bpl::PrintWriter& consoleWriter);
    };
}

#endif
