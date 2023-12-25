# Embedded-Linux (BTE5446)
# Project: Basic CMake Exercise Template
# Version: 1.0
# File:    CmakeLists.txt
# Date:    04.10.2015
# Author   Martin Aebersold (AOM1)
#
# Initial release: V1.0, AOM1, 04.10.2015
# Last Modifications: V1.1, AOM1, 27.07.2022
#
# CMake include file to cross compile for the ARM64 architecture.
#
# This can be used when running cmake in the following way:
#  mkdir build
#  cd build/
#  cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm-none-linux-aarch64-gcc.cmake 
#  make
#

# Target operating system name.
set(CMAKE_SYSTEM_NAME Linux)

# Set processor type
set(CMAKE_SYSTEM_PROCESSOR arm64)

# System platform
set(CMAKE_SYSTEM_PLATFORM aarch64-none-linux-gnu)

# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

# Set Cross-Toolchain path
set(CROSS_PATH /opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-${CMAKE_SYSTEM_PLATFORM})

# Compiler and tools
set(CMAKE_AR                        ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-ar${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER              ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_C_COMPILER                ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER              ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-g++${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_LINKER                    ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-ld${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY                   ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-objcopy${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_RANLIB                    ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-ranlib${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_SIZE                      ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-size${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_STRIP                     ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-strip${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_GCOV                      ${CROSS_PATH}/bin/${CMAKE_SYSTEM_PLATFORM}-gcov${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")

# Setting the various compiler flags
set(CMAKE_C_FLAGS                   "${APP_C_FLAGS} -Wno-psabi" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS                 "${APP_CXX_FLAGS} ${CMAKE_C_FLAGS}" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG             "-O0 -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "-O3 -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

# Set the target rootfs and sysroot path
set(TARGET_ROOTFS /opt/embedded/rootfs-rock5b)
set(TARGET_SYSROOT /opt/embedded/sysroot/arm-64/ubuntu-22.04-arm64)
include_directories(${TARGET_SYSROOT}/usr/include ${TARGET_SYSROOT}/usr/local/include ${TARGET_SYSROOT}/usr/include/aarch64-linux-gnu)
include_directories(${TARGET_SYSROOT}/usr/include/SDL2 ${TARGET_SYSROOT}/usr/include/drm)

# Sysroot library paths
link_directories(
        ${TARGRT_SYSROOT}/lib
        ${TARGET_SYSROOT}/usr/lib
        ${TARGET_SYSROOT}/usr/lib/${CMAKE_SYSTEM_PLATFORM}
        ${TARGET_SYSROOT}/usr/local/qt5-arm64/lib
)

# Adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment only.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search headers and libraries in the target environment only.
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set project specific C-Flags
set(CMAKE_C_FLAGS "-Wno-unused-parameter -Wno-unused-variable")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

# Set linker specific flags
set(CMAKE_EXE_LINKER_FLAGS "-Xlinker -rpath-link=${TARGET_SYSROOT}/usr/lib -Xlinker -rpath-link=${TARGET_SYSROOT}/usr/lib/arm-linux-gnueabihf -Xlinker -rpath-link=${TARGET_SYSROOT}/usr/local/lib \
		            -Xlinker -rpath-link=${TARGET_SYSROOT}/usr/lib/aarch64-linux-gnu/pulseaudio -Xlinker -rpath-link=${TARGET_SYSROOT}/lib/aarch64-linux-gnu")

# Exmaple how to send the -std=c99 flag to the gcc compiler
add_definitions(-std=c99)

