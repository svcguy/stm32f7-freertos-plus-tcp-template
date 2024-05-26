# STM32F7 with FreeRTOS and FreeRTOS-Plus-TCP Template
This template contains a skeleton project running on an STM32F767 Nucleo Board that uses FreeRTOS-Plus-TCP's TCP/IP stack.

It will simply flash the onboard green LED at 1Hz and start a DHCP client on the onboard ethernet interface.

VSCode is used as the 'IDE'.  The project uses the STM32 VSCode Extension to create and empty project.

CMake is then used via the wonderful stm32-cmake GitHub repository to fetch CMSIS and STM32F7 HAL.
CMake Fetch is used to get FreeRTOS and FreeRTOS-Plus-TCP from the GitHub repos.
STM32Cube CLT is the toolchain that provides GCC for compiling, STM32CubeProgrammer for flashing, ST-LINK GDBSERVER and GDB for debugging.
Requires the following VSCode Extensions: 
- [STM32 VSCode Extension](https://marketplace.visualstudio.com/items?itemName=stmicroelectronics.stm32-vscode-extension)
- [Microsoft C/C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [Microsoft CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [Cortex Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)

The STM32 VSCode extension will creates the tasks and debug configurations.  I added in debugging via a remote Raspberry Pi.
