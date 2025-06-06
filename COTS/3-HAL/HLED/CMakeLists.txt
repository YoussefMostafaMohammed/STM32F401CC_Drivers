cmake_minimum_required(VERSION 3.1...3.22)

project(HLED C)
set(CMAKE_C_COMPILER /usr/bin/avr-gcc)


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

# Source files
file(GLOB ${PROJECT_NAME}_SOURCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# set include directories path 
set(${PROJECT_NAME}_LIB_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/inc)   

# collect .h files
file(GLOB ${PROJECT_NAME}_LIB_INCLUDE_FILES ${${PROJECT_NAME}_LIB_INCLUDE_PATH}/*.h)

set(${PROJECT_NAME}_INC_FILES ${${PROJECT_NAME}_LIB_INCLUDE_FILES})

# Add library target
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCE_FILES} ${${PROJECT_NAME}_INC_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC MGPIO)
target_link_libraries(${PROJECT_NAME} PUBLIC MRCC)
target_link_libraries(${PROJECT_NAME} PUBLIC MNVIC)


# Include the directories
target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_LIB_INCLUDE_PATH})
