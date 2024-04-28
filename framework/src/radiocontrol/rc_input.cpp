//
// (c) Bit Parallel Ltd, April 2024
//

#include "framework/utils/string_utils.hpp"
#include "framework/radiocontrol/rc_input.hpp"

bpl::RcInput::RcInput(bpl::RcDecoder& rcDecoder):
    CliProvider("rc", "all | ch=#n | #ch=#n,#n,... | ch=#n:#n | stats [#repeat]"),
    rcDecoder(rcDecoder), useCyclicRing(false), aileron(0), elevator(0) {
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
    aileron = rcDecoder.getChannel(1);      // FIXME! these must use the correct mapped channel numbers
    elevator = rcDecoder.getChannel(2);
    if (useCyclicRing)
    {
        // use an iterative square root approximation to find the radius of the cyclic stick position
        // in order to re-scale the desired cyclic stick values and ensure that they lie within the cyclic ring
        // note, the calculated square root may be no more than 1 above or below the actual value
        //
        const auto magnitude = (aileron * aileron) + (elevator * elevator);
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
            aileron = (aileron << ABSOLUTE_MAX_CHANNEL_SHIFT) / radius;
            elevator = (elevator << ABSOLUTE_MAX_CHANNEL_SHIFT) / radius;
        }
    }
}

uint32_t bpl::RcInput::getThrottle() const
{
    return rcDecoder.getChannel(0);         // FIXME! this must use the correct mapped channel number
}

int32_t bpl::RcInput::getAileron() const
{
    return aileron;
}

int32_t bpl::RcInput::getElevator() const
{
    return elevator;
}

int32_t bpl::RcInput::getRudder() const
{
    return rcDecoder.getChannel(3);         // FIXME! this must use the correct mapped channel number
}

int32_t bpl::RcInput::getPitch() const
{
    return rcDecoder.getChannel(5);         // FIXME! this must use the correct mapped channel number
}

bpl::RcInputStatus bpl::RcInput::getStatus() const
{
    return rcDecoder.getStatus();
}

bpl::RcInputStatistics bpl::RcInput::getStatistics() const
{
    return rcDecoder.getStatistics();
}

bool bpl::RcInput::handleCliCommand(std::pmr::vector<std::string_view>& commandTokens, const bpl::TextIO& console, driver::Time& time)
{
    const auto& consoleWriter = console.getPrintWriter();
    const auto& consoleReader = console.getTextReader();

    // FIXME! add in the individual / range channel commands
    // allowed syntax: rc all | ch=#n | #ch=#n,#n,... | ch=#n:#n | stats [#repeat]
    //
    const auto all = commandTokens[1] == "all";
    const auto stats = commandTokens[1] == "stats";
    if (all | stats)
    {
        if (commandTokens.size() == 3)
        {
            int32_t refresh = 1;
            if (bpl::StringUtils::stoi(commandTokens[2], refresh))
            {
                // check every 1/4 second for an ESC key press, i.e. to quit
                //
                auto running = true;
                while (running)
                {
                    (all) ? printChannelValues(consoleWriter) : printChannelStatistics(consoleWriter);
                    consoleWriter.println();

                    for (auto interval = 0; interval < refresh * 4; interval++)
                    {
                        if (consoleReader.isKey(bpl::Ascii::ESC))
                        {
                            running = false;
                            break;
                        }

                        // wait for 1/4 second, time specified microseconds
                        //
                        time.spinWait(250'000);
                    }
                }

                return true;
            }
        }

        (all) ? printChannelValues(consoleWriter) : printChannelStatistics(consoleWriter);
        return true;
    }

    //
    // FIXME! parse and implement the remaining command options
    //

    return false;
}

void bpl::RcInput::printChannelValues(const bpl::PrintWriter& consoleWriter)
{
    // FIXME! these must use the correctly mapped channel numbers
    // note, cliChannelValueString has space for 19 + '\0' characters
    //
    consoleWriter.print("Throttle: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(rcDecoder.getChannel(0), cliStringBuffer));

    consoleWriter.print(" Alieron: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(rcDecoder.getChannel(1), cliStringBuffer));

    consoleWriter.print("Elevator: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(rcDecoder.getChannel(2), cliStringBuffer));

    consoleWriter.print("  Rudder: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(rcDecoder.getChannel(3), cliStringBuffer));

    consoleWriter.print("   Pitch: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(rcDecoder.getChannel(5), cliStringBuffer));
}

void bpl::RcInput::printChannelStatistics(const bpl::PrintWriter& consoleWriter)
{
    const auto statistics = rcDecoder.getStatistics();
    consoleWriter.println("RC Frame Stats");
    consoleWriter.print("     New: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(statistics.getNewDataCount(), cliStringBuffer));

    consoleWriter.print("   Stale: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(statistics.getStaleDataCount(), cliStringBuffer));

    consoleWriter.print("Failsafe: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(statistics.getFailsafeConditionCount(), cliStringBuffer));

    consoleWriter.print(" Unknown: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(statistics.getPacketErrorCount(), cliStringBuffer));

    consoleWriter.print("     CRC: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(statistics.getCrcErrorCount(), cliStringBuffer));

    consoleWriter.print("  Parity: ");
    consoleWriter.println(bpl::StringUtils::itoc<sizeof(cliStringBuffer)>(statistics.getParityErrorCount(), cliStringBuffer));
}
