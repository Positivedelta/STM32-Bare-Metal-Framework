//
// (c) Bit Parallel Ltd, December 2023
//

#include <array>
#include <cstdint>
#include <memory_resource>

#include "framework/cli/cli_handler.hpp"
#include "framework/drivers/usart2.hpp"
#include "framework/io/baud_rate.hpp"
#include "framework/io/text_io.hpp"
#include "framework/stm32f4/nvic.hpp"
#include "framework/tasking/scheduler.hpp"

// project includes
//
#include "led_driver.hpp"
#include "pinpoint_gyro_driver.hpp"
#include "sbus_driver.hpp"
#include "pwm_servo_driver.hpp"
#include "pid_controller.hpp"

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

    auto led = LedDriver(100, "LED Task");                                      // 100ms timebase
    auto gyros = PinPointGyroDriver(1, "Gyro Task", led);                       // sample every 1ms
    auto sbus = SBusDriver();
    auto servos = PWMServoDriver(333);                                          // update the servos or ESCs at 333Hz
    auto controller = PidController(3, "PID Task", gyros, sbus, servos, led);   // run the control loop every 3ms, i.e. at 333Hz
    auto& scheduler = bpl::TaskScheduler::getInstance().initialise(1'000, {gyros, controller, led}, Nvic::Priority10);
    scheduler.start();

    // FIXME! just for testing...
    //
    led.flash(5);

    // the CLI runs as the main foreground task
    // set the uart IRQ priority to 1 (0..15 ==> high..low)
    //
    auto& uart = driver::Usart2::getInstance().initialise(bpl::BaudRate::BPS_115200, Nvic::Priority1);
    const auto console = bpl::TextIO(uart.getInputStream(), uart.getOutputStream());
    auto cli = bpl::CliHandler(console, {led, gyros, sbus, servos, controller});
    while (true) cli.run();

    return 0;
}
