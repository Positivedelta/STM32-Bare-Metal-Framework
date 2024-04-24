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

//
// FIXME! must implement...
//        1, cli interface
//        2, channel mapping
//        3, flight mode
//

namespace bpl
{
    class RcInput : public bpl::CliProvider
    {
        public:
            static constexpr int32_t ABSOLUTE_MAX_CHANNEL_SHIFT = 12;
            static constexpr int32_t ABSOLUTE_MAX_CHANNEL_VALUE = 1 << 12;

        private:
            bpl::RcDecoder& rcDecoder;
            driver::Time& time;
            bool useCyclicRing;
            int32_t aileron, elevator;
            char rcChannelValueString[20];

        public:
            RcInput(bpl::RcDecoder& rcDecoder, driver::Time& time);

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
            bool handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console) override;
            void printChannelValues(const bpl::PrintWriter& consoleWriter, const bool addLine = false);
    };
}

#endif