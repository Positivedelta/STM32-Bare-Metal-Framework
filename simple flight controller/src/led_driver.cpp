//
// (c) Bit Parallel Ltd, December 2023
//

#include "framework/stm32f4/stm32f4.hpp"
#include "framework/stm32f4/rcc.hpp"
#include "framework/stm32f4/gpio.hpp"
#include "framework/utils/string_utils.hpp"

#include "led_driver.hpp"

LedDriver::LedDriver(const uint32_t period, const char* taskName):
    Task(period, taskName), CliProvider("led", "off | on | flash #period | activate"),
    ledPeriod(1), ledPeriodCount(0), isActive(true), ledOn(false), ledFlashing(false), ledToggle(true) {
        // enable the AHB1 clock, needed by gpioA
        //
        Stm32f4::rcc(Rcc::AHB1ENR) = Stm32f4::rcc(Rcc::AHB1ENR) | 1;
        asm("nop");

        // setup and initialise the Nucleo's user led2
        // make PA5 an output, it's connected to led2
        // set led2 off as it's default state
        //
        Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) & ~(Gpio::MODER_MASK << (Gpio::Pin5 << Gpio::MODER_SHIFT));
        Stm32f4::gpioA(Gpio::MODER) = Stm32f4::gpioA(Gpio::MODER) | (Gpio::OP << (Gpio::Pin5 << Gpio::MODER_SHIFT));
        Stm32f4::gpioA(Gpio::BSR) = 1 << (Gpio::Pin5 + 16);
}

//
// LedDriver methods
//

void LedDriver::on()
{
    if (isActive) doLedOn();
}

void LedDriver::off()
{
    if (isActive) doLedOff();
}

void LedDriver::flash(const uint32_t period)
{
    if (isActive) doLedFlash(period);
}

void LedDriver::doLedOn()
{
    ledFlashing = false;
    ledToggle = false;
    ledOn = true;
}

void LedDriver::doLedOff()
{
    ledFlashing = false;
    ledToggle = true;
    ledOn = false;
}

void LedDriver::doLedFlash(const uint32_t period)
{
    ledPeriod = (period == 0) ? 1 : period;
    ledPeriodCount = 0;
    ledFlashing = true;
}

void LedDriver::ifActiveSaveStateAndReportCliControl(const bpl::PrintWriter& consoleWriter)
{
    if (isActive)
    {
        currentLedPeriod = ledPeriod;
        currentLedPeriodCount = ledPeriodCount;
        currentLedOn = ledOn;
        currentLedFlashing = ledFlashing;
        currentLedToggle = ledToggle;

        consoleWriter.println("The LED is now under CLI control");
    }
}

void LedDriver::restoreActiveState()
{
    ledPeriod = currentLedPeriod;
    ledPeriodCount = currentLedPeriodCount;
    ledOn = currentLedOn;
    ledFlashing = currentLedFlashing;
    ledToggle = currentLedToggle;
}

//
// protected Task method
//

void LedDriver::runTask()
{
    if (ledFlashing)
    {
        ledPeriodCount = ledPeriodCount + 1;
        if (ledPeriodCount == ledPeriod)
        {
            Stm32f4::gpioA(Gpio::BSR) = 1 << ((ledToggle) ? Gpio::Pin5 : Gpio::Pin5 + 16);
            ledToggle = !ledToggle;

            ledPeriodCount = 0;
        }

        return;
    }

    Stm32f4::gpioA(Gpio::BSR) = 1 << ((ledOn) ? Gpio::Pin5 : Gpio::Pin5 + 16);
}

//
// protected CliProvider method
//

bool LedDriver::doExecute(std::pmr::vector<std::string_view>& commandTokens, const bpl::PrintWriter& consoleWriter)
{
    // allowed syntax: led off | on | flash #period | activate
    //
    if (commandTokens.front() == getName())
    {
        if (commandTokens[1] == "off")
        {
            ifActiveSaveStateAndReportCliControl(consoleWriter);

            isActive = false;
            doLedOff();
            consoleWriter.println("Set: OFF");

            return true;
        }

        if (commandTokens[1] == "on")
        {
            ifActiveSaveStateAndReportCliControl(consoleWriter);

            isActive = false;
            doLedOn();
            consoleWriter.println("Set: ON");

            return true;
        }

        if ((commandTokens[1] == "flash") && (commandTokens.size() == 3))
        {
            int32_t period = 0;
            const auto validPeriod = (commandTokens[2].size() < 4) && bpl::StringUtils::stoi(commandTokens[2], period);
            if (validPeriod)
            {
                ifActiveSaveStateAndReportCliControl(consoleWriter);

                isActive = false;
                doLedFlash(uint32_t(period));
                consoleWriter.print("Set: FLASH = ");
                consoleWriter.println(commandTokens[2]);

                return true;
            }

            return false;
        }

        if (commandTokens[1] == "activate")
        {
            if (!isActive)
            {
                restoreActiveState();

                isActive = true;
                consoleWriter.println("Relinquished LED control, original state restored");
            }
            else
            {
                consoleWriter.println("Already active");
            }

            return true;
        }

        return false;
    }

    return false;
}
