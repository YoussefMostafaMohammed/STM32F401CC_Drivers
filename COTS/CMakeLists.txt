cmake_minimum_required(VERSION 3.1...3.22)

# Project Configuration
project(Test_GPIO C)
set(MCU atmega32)       # Target microcontroller
set(F_CPU 8000000UL)    # Clock frequency

#---------------------------------------
# AVR Toolchain Configuration
#---------------------------------------
set(CMAKE_SYSTEM_NAME Generic)

# Build libraries first
add_subdirectory(2-MCAL/MGPIO)
add_subdirectory(2-MCAL/MRCC)
add_subdirectory(2-MCAL/MNVIC)
add_subdirectory(3-HAL/HLED)



set(CMAKE_C_COMPILER /usr/bin/avr-gcc)
set(MCU atmega32)       # Set your microcontroller
set(PROGRAMMER usbasp)    # Set your programmer ID

# AVR-specific paths
find_path(AVR_INCLUDE_DIR avr/io.h
  PATHS /usr/lib/avr/include /usr/avr/include
  REQUIRED
)

# Compiler/Linker Flags
#---------------------------------------------------
add_compile_options(
  -mmcu=${MCU}
  -DF_CPU=${F_CPU}
  -Os
  -Wall
  -Wextra
  -ffunction-sections
  -fdata-sections
  -I${AVR_INCLUDE_DIR}  # Explicitly include AVR headers
)

add_link_options(
  -mmcu=${MCU}
  -Wl,--gc-sections     # Remove unused sections
)

# Prevent host system headers from being used
include_directories(${AVR_INCLUDE_DIR})
set(CMAKE_C_FLAGS "-nostdinc")

#---------------------------------------
# Project Structure
#---------------------------------------
if(${PROJECT_NAME} STREQUAL "Test_GPIO")
    set(AssignmentDir Assignment_1)
elseif(${PROJECT_NAME} STREQUAL "Test_LED")
    set(AssignmentDir Assignment_2)
endif()
#---------------------------------------
# Source Files and Targets
#---------------------------------------
file(GLOB APP_SOURCES 
  ${CMAKE_SOURCE_DIR}/4-APP/${AssignmentDir}/${PROJECT_NAME}/*.c
)

# Main executable
add_executable(${PROJECT_NAME}.elf ${APP_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}.elf 
  PRIVATE 
  MGPIO
  MRCC
  MNVIC
  HLED
)

#---------------------------------------
# Flashing/Burning
#---------------------------------------
include(${CMAKE_SOURCE_DIR}/CmakeScript.cmake)
FlashAndBurn()