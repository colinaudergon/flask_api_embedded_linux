# Embedded-Linux (BTE5446)
# Project: Basic CMake Exercise
# Version: 1.0
# File:    CmakeLists.txt
# Date:    04.10.2015
# Author   Martin Aebersold (AOM1)
#
# Initial release: V1.0, AOM1, 04.10.2015
# Last Modifications: V1.1, AOM1, 27.07.2022
# Adapted for ARM32 amd ARM64
#
# This config file can be used when running cmake in the following way:
#
#  $ mkdir build
#  $ cd build/
#
#  To Cross-Compile for the ARM32 architecture run:
#  $ cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm-none-linux-gnueabihf-gcc.cmake
#
#  To Cross-Compile for the ARM64 architecture run:
#  $ cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm-none-linux-aarch64-gcc.cmake
#
#  Native-Compile run:
#  $ cmake .. -DCMAKE_TOOLCHAIN_FILE=../native-linux-gcc.cmake
#
#  It will generate the Makefile now you may run make:
#  $ make
#  
#  To install the App on the target use:
#  $ make install
#
# If you like to use a GUI frontend run the following commands:
# $ sudo apt install cmake-qt-gui
# $ cmake-qt-gui

# This one is important
set(CMAKE_SYSTEM_NAME Linux)

# This one not so much
set(CMAKE_SYSTEM_VERSION 1)

# Specify the version being used as well as the language
cmake_minimum_required(VERSION 3.7)

# Name your project here
project(inputReading)

# Project-wide compilation options.
add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter $<$<COMPILE_LANGUAGE:C>:-std=c17> $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>)

# Header and Sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB_RECURSE HEADERS "*.hpp" "*.h")
file(GLOB SOURCES
	"src/*.c"
        "src/*.cpp"
        "src/*.h"
)
include_directories(
        "include"
        "/usr/include"
)

# Executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Add libaries to be linked with
if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm32")
 target_link_libraries(${CMAKE_PROJECT_NAME}
 	pthread
 	rt
 	m
 	drm
 )
 elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
 target_link_libraries(${CMAKE_PROJECT_NAME}
 	pthread
 	rt
 	m
 	drm
 )
else (CMAKE_SYSTEM_PROCESSOR MATCHES "intel")
 target_link_libraries(${CMAKE_PROJECT_NAME}
 	pthread
 	rt
 	m
 	drm
 ) 
endif ()

# To keep assembly output
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-save-temps=obj")
set(CMAKE_C_FLAGSS "${CMAKE_C_FLAGSS}-save-temps=obj")

# Check where to install the binary (Platform dependend)
if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm32")
  # Cross (ARM32) target install path for the App
  install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${TARGET_ROOTFS}/home/student/Apps)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
  # Cross (ARM64) target install path for the App    
  install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${TARGET_ROOTFS}/home/student/Apps)  
else (CMAKE_SYSTEM_PROCESSOR MATCHES "intel")
  # Native target install path for the App
  install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION /usr/local/bin)
endif ()

# Is doxygen installed?
FIND_PACKAGE(Doxygen)

# If doxygen is installed create the documenation
if (DOXYGEN_FOUND)

set(DOXYGEN_INPUT doxyfile)
set(DOXYGEN_OUTPUT build/doc)

ADD_CUSTOM_COMMAND(
  OUTPUT ${DOXYGEN_OUTPUT}
  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
  COMMAND ${CMAKE_COMMAND} -E echo "Done."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${DOXYGEN_INPUT}
  )

ADD_CUSTOM_TARGET(apidoc ALL DEPENDS ${DOXYGEN_OUTPUT})

ADD_CUSTOM_TARGET(apidoc_forced
  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
  COMMAND ${CMAKE_COMMAND} -E echo "Done."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endif (DOXYGEN_FOUND)

