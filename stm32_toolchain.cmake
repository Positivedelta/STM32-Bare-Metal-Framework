set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# tests the compiler without invoking the linker, this is problematic when using a custom linker script
#
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# FIXME! figure out how to remove -fno-exceptions and link to resolve the abort() call
#
# notes 1, adding all of the specific STM32 compiler and linker options here
#       2, the required linker script is provided in the CMakeLists.txt file
#
set(CMAKE_CXX_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-exceptions -fno-use-cxa-atexit --specs=nano.specs")
set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float -lstdc++_nano -lsupc++_nano -lc_nano -Wl,--gc-sections")
