#
# (c) Bit Parallel Ltd, September 2023
#

cmake_minimum_required(VERSION 3.27)

# notes 1, the PROJECT_SOURCE_DIR variable is only set after the project() directive and can't be used to set the CMAKE_TOOLCHAIN_FILE variable
#       2, to avoid this use the parent of CMAKE_CURRENT_LIST_DIR as this is available before calling project()
#       3, the CMAKE_GENERATOR variable has to be set in PreLoad.cmake, setting it here is too late (you'll get Visual Studio, the default builder for windows)
#       4, if PreLoad.cmake is ever deprecated use: cmake -G "NMake Makefiles" ..
#
cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH BIT_PARALLEL_STM32_ROOT)
set(CMAKE_TOOLCHAIN_FILE ${BIT_PARALLEL_STM32_ROOT}/stm32_toolchain.cmake)
project("nucleo_f446re_simple_flight_controller" CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3")

include_directories(${BIT_PARALLEL_STM32_ROOT}/framework/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB STM32F4_SRC ${BIT_PARALLEL_STM32_ROOT}/framework/src/stm32f4/*.cpp)
file(GLOB DRIVERS_SRC ${BIT_PARALLEL_STM32_ROOT}/framework/src/drivers/*.cpp)
file(GLOB TASKING_SRC ${BIT_PARALLEL_STM32_ROOT}/framework/src/tasking/*.cpp)
file(GLOB IO_SRC ${BIT_PARALLEL_STM32_ROOT}/framework/src/io/*.cpp)
file(GLOB CLI_SRC ${BIT_PARALLEL_STM32_ROOT}/framework/src/cli/*.cpp)
file(GLOB UTILS_SRC ${BIT_PARALLEL_STM32_ROOT}/framework/src/utils/*.cpp)
file(GLOB NUCLEO_SIMPLE_FLIGHT_CONTROLLER_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(APPEND NUCLEO_SIMPLE_FLIGHT_CONTROLLER_SRC ${STM32F4_SRC} ${DRIVERS_SRC} ${TASKING_SRC} ${IO_SRC} ${CLI_SRC} ${UTILS_SRC})
add_executable(${CMAKE_PROJECT_NAME} ${NUCLEO_SIMPLE_FLIGHT_CONTROLLER_SRC})

# notes 1, the --no-warn-rwx-segments can be removed once the compiler is upgraded to v13, see the GCC v12 #1029 issue
#       2, added extra quotes to cater for spaces in paths
#
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE "LINKER:--no-warn-rwx-segments")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T \"${BIT_PARALLEL_STM32_ROOT}/framework/linker/stm32f446re_ldscript.ld\"")
target_link_libraries(${CMAKE_PROJECT_NAME})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}.elf")

# convert the .elf to .bin and .hex files then generate a .asm assembly listing file
#
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJDUMP} ARGS -D ${CMAKE_PROJECT_NAME}.elf > ${CMAKE_PROJECT_NAME}.asm)

# the .hex file is automatically added to the 'clean' target, but not the above .bin, .hex and .asm files
#
set_property(TARGET ${CMAKE_PROJECT_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.bin ${CMAKE_PROJECT_NAME}.hex ${CMAKE_PROJECT_NAME}.asm)
