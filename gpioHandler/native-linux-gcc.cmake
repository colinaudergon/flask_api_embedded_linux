# Embedded-Linux (BTE5446)
# Project: Basic CMake Exercise
# Version: 1.0
# File:    CmakeLists.txt
# Date:    04.10.2015
# Author   Martin Aebersold (AOM1)
#
# Last Modifications: V1.0, AOM1, 04.10.2015
# Initial release
#
# CMake include file native compiling.
#
# This can be used when running cmake in the following way:
#  mkdir build
#  cd build/
#  cmake .. -DCMAKE_TOOLCHAIN_FILE=../native-linux-gcc.cmake
#  make
#

# Target operating system name.
set(CMAKE_SYSTEM_NAME Linux)

# Set processor type
set(CMAKE_SYSTEM_PROCESSOR intel)

set(CMAKE_AR                        gcc-ar${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER              gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_C_COMPILER                gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER              g++${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_LINKER                    ld${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY                   objcopy${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_RANLIB                    ranlib${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_SIZE                      size${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_STRIP                     strip${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_GCOV                      gcov${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")

set(CMAKE_C_FLAGS                   "${APP_C_FLAGS} -Wno-psabi" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS                 "${APP_CXX_FLAGS} ${CMAKE_C_FLAGS}" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG             "-O0 -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "-O3 -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

# Where to look for the target environment. (More paths can be added here)
set(CMAKE_FIND_ROOT_PATH "/")

# Set the target rootfs path
include_directories(/usr/include /usr/include/drm /usr/local/include)

# Adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment only.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search headers and libraries in the target environment only.
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set project specific C-Flags
set(CMAKE_C_FLAGS "-Wno-unused-parameter -Wno-unused-variable")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

# Sends the -std=c99 flag to the gcc compiler
add_definitions(-std=c99)

