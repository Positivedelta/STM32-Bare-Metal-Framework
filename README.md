# An STM32 Bare Metal C++ Framework with Examples
This repository contains an experimental bare metal framework written in C++ for use with STM32 microcontrollers. Everything here is subject
to change and will hopefully evolve into something useful

Currently the examples run on an STM32 Nucleo-64 evaluation board, see https://www.st.com/en/evaluation-tools/nucleo-f446re.html for details of the specific board used whilst developing this project

Note, this project has been set up to be cross compiled from a Windows machine. However, it's not difficult to tweak the build environment and use a Linux box

It is assumed that you have some knowledge of embedded programming on STM32 or similar microcontrollers and are familiar with C++

This project was inspired by the Jason Turner talk at CppCon 2016: "Rich Code for Tiny Computers: A Simple Commodore 64 Game in C++17", see https://www.youtube.com/watch?v=zBkNBP00wJE This is an excellent talk and is well worth watching, as is his "C++ Weekly" channel

**Note**, the `simple flight controller` project is incomplete. When it's finished it will be able to fly an RC Quadcopter using the Nucleo-64 board described above. This board will also need a
plugin hat to host the 3-axis gyros, the RC interface and the speed controller PWM connectors. The KiCad PCB design and layout will be included here once it's complete. Watch this space for updates...

## Prerequisites
- CMake v3.27 (older versions are probably OK)
- Nmake v14.34.x
- GCC 13.2.1

Notes
- Microsoft `nmake` is installed as a component of Visual Studio but is also available separately as a download
- If building on a Linux box, edit the `PreLoad.cmake` files to specify `Unix Makefiles` and then use `make`
- The mingw-w64-i686 AArch32 bare-metal target (arm-none-eabi) GCC 12.3 compiler was used
- A Linux version of the CGG cross compiler is also available
- These can be download from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
- If necessary edit the `stm32_toolchain.cmake` file to suit your cross compiler toolchain

## Build
In the desired example directory perform the following steps
- `mkdir build`
- `cd build`
- `cmake ..`
- `nmake`

## Execute
- Plug the Nucleo into a PC, it will mount a drive
- Drag the .bin file to the drive mounted by the Nucleo
- The code should now execute on the Nucleo

Notes
- Use the generated .hex file with a JTAG/SWD style programmers
- The compiled assembler is available in the .asm file

## Help
Please feel free to ask questions via max.vandaalen@bitparallel.com
