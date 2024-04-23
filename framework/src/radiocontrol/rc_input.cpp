//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/radiocontrol/rc_input.hpp"

bpl::RcInput::RcInput(bpl::RcDecoder& rcDecoder):
    CliProvider("rc", "[repeat] all | #ch | #ch #ch ..#ch | #ch:#ch"),
    rcDecoder(rcDecoder), useCyclicRing(false), alieron(0), elevator(0) {
}

void bpl::RcInput::setCyclicRing(const bool enable)
{
    useCyclicRing = enable;
}

bool bpl::RcInput::hasCyclicRing() const
{
    return useCyclicRing;
}

void bpl::RcInput::aquire()
{
    // grab the normalised channels, expected to be values in the range [-4096..4096] or [0..4096] for the throttle
    // FIXME! 1, check the return value and handle failsafe
    //        2, extract and add methods to return the status and statistics, currently these are not generic
    //
    rcDecoder.decode();

    //
    // FIXME! remap the channels as per the setup configuration
    //

    // apply a cyclic ring if enabled
    //
    alieron = rcDecoder.getChannel(1);      // FIXME! these must use the correct mapped channel numbers
    elevator = rcDecoder.getChannel(2);
    if (useCyclicRing)
    {
        // FIXME! this needs testing!
        //
        // use an iterative square root approximation to find the radius of the cyclic stick position
        // in order to re-scale the desired cyclic stick values and ensure that they lie within the cyclic ring
        // note, the calculated square root may be no more than 1 above or below the actual value
        //
        const auto magnitude = (alieron * alieron) + (elevator * elevator);
        if (magnitude > ABSOLUTE_MAX_CHANNEL_VALUE * ABSOLUTE_MAX_CHANNEL_VALUE)
        {
            auto extra = ABSOLUTE_MAX_CHANNEL_VALUE >> 2;
            auto searchValue = extra >> 1;
            while (searchValue > 0)
            {
                auto testRadius = ABSOLUTE_MAX_CHANNEL_VALUE + extra;
                auto testRadiusSquared = testRadius * testRadius;
                if (testRadiusSquared > magnitude)
                {
                    extra -= searchValue;
                    searchValue >>= 1;
                }
                else if (testRadiusSquared < magnitude)
                {
                    extra += searchValue;
                    searchValue >>= 1;
                }
                else
                {
                    // found the exact value, finish early
                    //
                    break;
                }
            }

            // scale by (ABSOLUTE_MAX_CHANNEL_VALUE / radius) where the radius is ABSOLUTE_MAX_CHANNEL_VALUE + extra
            //
            const auto radius = ABSOLUTE_MAX_CHANNEL_VALUE + extra;
            alieron = (alieron << ABSOLUTE_MAX_CHANNEL_SHIFT) / radius;
            elevator = (elevator << ABSOLUTE_MAX_CHANNEL_SHIFT) / radius;
        }
    }
}

uint32_t bpl::RcInput::getThrottle() const
{
    return rcDecoder.getChannel(0);         // FIXME! this must use the correct mapped channel number;
}

int32_t bpl::RcInput::getAileron() const
{
    return alieron;
}

int32_t bpl::RcInput::getElevator() const
{
    return elevator;
}

int32_t bpl::RcInput::getRudder() const
{
    return rcDecoder.getChannel(3);         // FIXME! this must use the correct mapped channel number;
}

int32_t bpl::RcInput::getPitch() const
{
    return rcDecoder.getChannel(5);         // FIXME! this must use the correct mapped channel number;
}

//void RcInput::irq()
//{
//}

bool bpl::RcInput::handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console)
{
    const auto& consoleWriter = console.getPrintWriter();

    // FIXME! parse the sbus command and display the results
    //
    consoleWriter.println("The RC command needs implementing...");
    return true;
}
