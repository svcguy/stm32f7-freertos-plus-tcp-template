cmake_minimum_required(VERSION 3.22)

#
# User is free to modify the file as much as necessary
#
include(FetchContent)
include("cmake/gcc-arm-none-eabi.cmake")

# Get stm32-cmake
FetchContent_Declare(stm32-cmake
  GIT_REPOSITORY https://github.com/ObKo/stm32-cmake.git
  GIT_TAG        master
)
# Get freertos-kernel
FetchContent_Declare(freertos_kernel
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
  GIT_TAG        main
)
# Get freertos-plus-tcp
FetchContent_Declare(freertos_plus_tcp
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Plus-TCP.git
  GIT_TAG        main
  GIT_SUBMODULES ""
)

# Core project settings
project(stm32f7-freertos-plus-tcp-template)
enable_language(C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Core MCU flags, CPU type, instruction set and FPU setup
set(cpu_PARAMS)

# Sources
set(sources_SRCS
    # We need system_stm32f7xx.c but can't include it through stm32-cmake
    # because that will also bring in the startup_stm32xxxxxx.s which
    # is being provided by the vscode extension cmake script
    "build/$<CONFIG>/_deps/stm32-cmsis-f7-src/Source/Templates/system_stm32f7xx.c"
    Src/stm32f7xx_it.c
    Src/clock.c
    Src/gpio.c
    Src/eth.c
    Src/rng.c
    Src/rtos.c
    Src/network.c
    Src/uart.c
)

# Include directories for all compilers
set(include_DIRS)

# Include directories for each compiler
set(include_c_DIRS)
set(include_cxx_DIRS)
set(include_asm_DIRS)

# Symbols definition for all compilers
set(symbols_SYMB
    STM32F767xx
)

# Symbols definition for each compiler
set(symbols_c_SYMB)
set(symbols_cxx_SYMB)
set(symbols_asm_SYMB)

# Link directories and names of libraries
set(link_DIRS)
set(link_LIBS
    CMSIS::STM32::F7
    HAL::STM32::F7::CORTEX
    HAL::STM32::F7::RCC
    HAL::STM32::F7::RCCEx
    HAL::STM32::F7::PWREx
    HAL::STM32::F7::FLASH
    HAL::STM32::F7::GPIO
    HAL::STM32::F7::TIM
    HAL::STM32::F7::TIMEx
    HAL::STM32::F7::RNG
    HAL::STM32::F7::UART
    # Notice no HAL::STM32::F7::ETH - FreeRTOS-Plus-TCP provides
    # the HAL implemenation
    freertos_kernel
    freertos_plus_tcp
)

# Linker script
set(linker_script_SRC)

# Compiler options
set(compiler_OPTS)

# Linker options
set(linker_OPTS)

############################
#### STM32 HAL settings ####
############################

# Bring in stm32-cmake
FetchContent_MakeAvailable(stm32-cmake)

# Include stm32-cmake to enable next commands
# TODO: make this more robust
include("build/debug/_deps/stm32-cmake-src/cmake/stm32_gcc.cmake")

# Get HAL and CMSIS
stm32_fetch_cmsis(F7)
stm32_fetch_hal(F7)

# We only want the generic F7 stuff, not anything processor specific.
find_package(CMSIS COMPONENTS STM32F7 REQUIRED)
find_package(HAL COMPONENTS STM32F7 REQUIRED)

###########################
#### FreeRTOS settings ####
###########################

# FreeRTOSConfig.h comes in as it's own target, define
#   the targert and tell CMake where the files are
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE
    Inc
)

# Set values required by FreeRTOS
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_ARM_CM7" CACHE STRING "" FORCE)

# Bring in freertos-kernel
FetchContent_MakeAvailable(freertos_kernel)

# stm32-cmake knows stuff about the processor so let's get
#   the compiler options and definitions from it to give to
#   freertos-kernel and freertos-kernel-port
get_target_property(STM32_HAL_COMPILER_OPTS STM32::F7 INTERFACE_COMPILE_OPTIONS)
get_target_property(STM32_HAL_COMPILER_DEFS STM32::F7 INTERFACE_COMPILE_DEFINITIONS)
get_target_property(STM32_CMSIS_COMPILER_DEFS STM32::F767xx INTERFACE_COMPILE_DEFINITIONS)

target_compile_options(freertos_kernel PRIVATE
    ${STM32_HAL_COMPILER_OPTS}
)
target_compile_definitions(freertos_kernel PRIVATE
    ${STM32_HAL_COMPILER_DEFS}
    ${STM32_CMSIS_COMPILER_DEFS}
)
target_compile_options(freertos_kernel_port PRIVATE
    ${STM32_HAL_COMPILER_OPTS}
)
target_compile_definitions(freertos_kernel_port PRIVATE
    ${STM32_HAL_COMPILER_DEFS}
    ${STM32_CMSIS_COMPILER_DEFS}
)

# Set values required by freertos_plus_tcp
set(FREERTOS_PLUS_TCP_NETWORK_IF "STM32FXX" CACHE STRING "" FORCE)

# Bring in freertos_plus_tcp
FetchContent_MakeAvailable(freertos_plus_tcp)

# Apply the compiler options and definitons we got earlier to
#   freertos_plus_tcp
target_compile_options(freertos_plus_tcp PRIVATE
    ${STM32_HAL_COMPILER_OPTS}
)
target_compile_definitions(freertos_plus_tcp PRIVATE
    ${STM32_HAL_COMPILER_DEFS}
    ${STM32_CMSIS_COMPILER_DEFS}
)
target_compile_options(freertos_plus_tcp INTERFACE
    ${STM32_HAL_COMPILER_OPTS}
)
target_compile_definitions(freertos_plus_tcp INTERFACE
    ${STM32_HAL_COMPILER_DEFS}
    ${STM32_CMSIS_COMPILER_DEFS}
    STM32F7xx
)

# freertos-plus-tcp's network interface needs to know where to find the rest of
#   the HAL and CMSIS includes, so let's get them and then provide them
get_target_property(HAL_INCLUDES HAL::STM32::F7 INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(CMSIS_INCLUDES CMSIS::STM32::F7 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(freertos_plus_tcp INTERFACE
    ${HAL_INCLUDES}
    ${CMSIS_INCLUDES}
)
target_include_directories(freertos_plus_tcp_port INTERFACE
    ${HAL_INCLUDES}
    ${CMSIS_INCLUDES}
)

# Now call generated cmake
# This will add script generated
# information to the project
include("cmake/vscode_generated.cmake")

# Link directories setup
# Must be before executable is added
link_directories(${CMAKE_PROJECT_NAME} ${link_DIRS})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${sources_SRCS})

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${include_DIRS}
    $<$<COMPILE_LANGUAGE:C>: ${include_c_DIRS}>
    $<$<COMPILE_LANGUAGE:CXX>: ${include_cxx_DIRS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${include_asm_DIRS}>
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    ${symbols_SYMB}
    $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
    $<$<COMPILE_LANGUAGE:CXX>: ${symbols_cxx_SYMB}>
    $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

    # Configuration specific
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>: >
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${link_LIBS})

# Compiler options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    ${cpu_PARAMS}
    ${compiler_OPTS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:C>: >
    $<$<COMPILE_LANGUAGE:CXX>:

    # -Wno-volatile
    # -Wold-style-cast
    # -Wuseless-cast
    # -Wsuggest-override
    >
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

# Linker options
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T${linker_script_SRC}
    ${cpu_PARAMS}
    ${linker_OPTS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -u _printf_float # STDIO float formatting support (remove if not used)
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,-z,max-page-size=8 # Allow good software remapping across address space (with proper GCC section making)
    -Wl,--print-memory-usage
)

# Execute post-build to print size, generate hex and bin
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)
