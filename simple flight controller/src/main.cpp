//
// (c) Bit Parallel Ltd, October 2023
//

#include <array>
#include <cstdint>
#include <memory_resource>

// framework includes
//
#include "stm32_nvic.hpp"
#include "baud_rate.hpp"
#include "text_io.hpp"

// project includes
//
#include "led_driver.hpp"
#include "pinpoint_gyro_driver.hpp"
#include "sbus_driver.hpp"
#include "pwm_servo_driver.hpp"
#include "pid_controller.hpp"
#include "systick_handler.hpp"
#include "usart2_handler.hpp"
#include "cli_handler.hpp"

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

    constexpr auto TIME_BASE = 500;
    auto&& led = LedDriver(200 * TIME_BASE);                                    // 100ms timebase
    auto&& gyros = PinPointGyroDriver(TIME_BASE, led);                          // sample every 500us
    auto&& sbus = SBusDriver();
    auto&& servos = PWMServoDriver(333);                                        // update the servos or ESCs at 333Hz
    auto&& controller = PidController(6 * TIME_BASE, gyros, sbus, servos, led); // run the control loop every 3ms, i.e. at 333Hz
//  auto&& pid = PidController(gyros, sbus, servos, led);
//  auto&& controller = PendSV(3000 * TIME_BASE, pid);                          // run the control loop every 3ms, i.e. at 333Hz

    auto sysTick = SysTick::getInstance(TIME_BASE, {led, gyros, controller}, Nvic::Priority2);
    sysTick.enable();

    // FIXME! just for testing...
    //
    led.flash(5);

    // the CLI runs as the main foreground task
    // set the uart IRQ priority to 1 (0..15 ==> high..low)
    //
    auto uart = Usart2IRQ::getInstance(bpl::BaudRate::BPS_115200, Nvic::Priority1);
    const auto console = bpl::TextIO(uart.getInputStream(), uart.getOutputStream());
    auto cli = CliHandler(console, {gyros, sbus, servos, controller});
    while (true) cli.run();

    return 0;
}
