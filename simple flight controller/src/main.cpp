//
// (c) Bit Parallel Ltd, December 2023
//

#include <array>
#include <cstdint>
#include <memory_resource>

#include "framework/cli/cli_handler.hpp"
#include "framework/drivers/time.hpp"
#include "framework/drivers/usart1.hpp"
#include "framework/drivers/usart2.hpp"
#include "framework/io/baud_rate.hpp"
#include "framework/radiocontrol/rc_input.hpp"
#include "framework/radiocontrol/spektrum_srxl_decoder.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/tasking/scheduler.hpp"

// project includes
//
#include "led_driver.hpp"
#include "pinpoint_gyro_driver.hpp"
#include "pwm_servo_driver.hpp"
#include "pid_controller.hpp"

//
// IRQ priorities (0..15 ==> high..low)
//
//   1: RSXL RC receiver input UART
//   2: console UART
//   3: time provider
//  10: task scheduler
//

int main()
{
    // FIXME! 1, setup appropriate options
    //        2, move this code somewhere better?
    // setup a PMR allocator for use with vectors, maps, ...
    //
    auto pmrBuffer = std::array<uint8_t, 16384>();
    auto upstreamResource = std::pmr::null_memory_resource();
    auto monotonicPool = std::pmr::monotonic_buffer_resource(pmrBuffer.data(), pmrBuffer.size(), upstreamResource);
    auto pool = std::pmr::unsynchronized_pool_resource(&monotonicPool);
    std::pmr::set_default_resource(&pool);

    // set the NVIC to use group4, i.e. 4 bits for preemption, 0 bits for sub priority
    // note, this application only uses a small number of IRQ priority levels, i.e. no need for sub-levels
    //
    Nvic::setPriorityGrouping(Nvic::PRE4_SUB0);

    // FIXME! 1, instantiating the time reference here as it might required elsewhere
    //        2, eventually the RcDecoder instances need to be configurable, i.e. construct them all and select an instance...
    //
    auto& time = driver::Time::getInstance(90'000'000, Nvic::Priority3);            // based on a 90 Mhz timebase (the APB1 bus clock speed)
    auto& rsxl = driver::Usart1::getInstance().initialise(bpl::BaudRate::BPS_115200, Nvic::Priority1);
    auto rcDecoder = bpl::SpektrumSrxlDecoder(rsxl, time);
    auto rcInput = bpl::RcInput(rcDecoder);
//  auto rcInput = bpl::RcInput(bpl::SpektrumSrxlDecoder(rsxl, time));

    auto led = LedDriver(100, "LED Task");                                          // 100ms timebase
    auto gyros = PinPointGyroDriver(1, "Gyro Task", led);                           // sample every 1ms
    auto servos = PWMServoDriver(333);                                              // update the servos or ESCs at 333Hz
    auto controller = PidController(3, "PID Task", gyros, rcInput, servos, led);    // run the control loop every 3ms, i.e. at 333Hz
    auto& scheduler = bpl::TaskScheduler::getInstance().initialise(1'000, {gyros, controller, led}, Nvic::Priority10);
    scheduler.start();

    // FIXME! just for testing...
    //
    led.flash(5);

    // the CLI runs as the main foreground task
    //
    const auto& console = driver::Usart2::getInstance().initialise(bpl::BaudRate::BPS_115200, Nvic::Priority2);
    auto cli = bpl::CliHandler(console, {scheduler, led, gyros, rcInput, servos, controller}, time);
    while (true) cli.run();

    return 0;
}
