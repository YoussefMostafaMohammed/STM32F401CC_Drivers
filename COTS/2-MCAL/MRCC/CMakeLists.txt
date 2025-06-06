cmake_minimum_required(VERSION 3.1...3.22)

project(MRCC)
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
set(STDLIB_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/0-STD_LIBS/inc)
set(COMMON_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/1-common/inc/MRCC)

# collect .h files
file(GLOB ${PROJECT_NAME}_LIB_INCLUDE_FILES ${${PROJECT_NAME}_LIB_INCLUDE_PATH}/*.h)
file(GLOB STDLIB_INCLUDE_FILES ${STDLIB_INCLUDE_PATH}/*.h)
file(GLOB COMMON_INCLUDE_FILES ${COMMON_INCLUDE_PATH}/*.h)

set(${PROJECT_NAME}_INC_FILES ${${PROJECT_NAME}_LIB_INCLUDE_FILES} ${STDLIB_INCLUDE_FILES} ${COMMON_INCLUDE_FILES})

# Add library target
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCE_FILES} ${${PROJECT_NAME}_INC_FILES})

# Include the directories
target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_LIB_INCLUDE_PATH})
target_include_directories(${PROJECT_NAME} PUBLIC ${STDLIB_INCLUDE_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_PATH})
