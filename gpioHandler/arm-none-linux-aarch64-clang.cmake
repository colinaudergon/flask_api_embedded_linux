# Target operating system name.
set(CMAKE_SYSTEM_NAME Linux)

# Set processor type
set(CMAKE_SYSTEM_PROCESSOR arm64)

# System platform
set(CMAKE_SYSTEM_PLATFORM aarch64-none-linux-gnu)

# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

set(CMAKE_AR                        llvm-ar-14${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER              clang-14${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_C_COMPILER                clang-14${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER              clang++-14${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_LINKER                    lld-14${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY                   llvm-objcopy-14${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_RANLIB                    llvm-ranlib-14${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_SIZE                      llvm-size-14${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_STRIP                     llvm-strip-14${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_GCOV                      llvm-cov-14${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")

# Set Cross-Toolchain path
set(TOOLCHAIN_ROOT /opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-${CMAKE_SYSTEM_PLATFORM})

set(CMAKE_C_FLAGS                   "${APP_C_FLAGS} -target aarch64-linux-gnu --gcc-toolchain=${TOOLCHAIN_ROOT} --sysroot=${TOOLCHAIN_ROOT}/aarch64-none-linux-gnu/libc" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS                 "${APP_CXX_FLAGS} ${CMAKE_C_FLAGS}" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG             "-O0 -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "-O3 -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS          "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=${TOOLCHAIN_ROOT}/bin/aarch64-none-linux-gnu-ld")

# Set the rootfs and sysroot path
set(TARGET_ROOTFS /opt/embedded/rootfs-rock5b)
set(TARGET_SYSROOT /opt/embedded/sysroot/arm-64/ubuntu-22.04-arm64)
include_directories(${TARGET_SYSROOT}/usr/include ${TARGET_SYSROOT}/usr/local/include ${TARGET_SYSROOT}/usr/include/aarch64-linux-gnu)
include_directories(${TARGET_SYSROOT}/usr/include/SDL2 ${TARGET_SYSROOT}/usr/include/drm)

# Sysroot library paths
link_directories(
        ${TARGRT_SYSROOT}/lib
        ${TARGET_SYSROOT}/usr/lib
        ${TARGET_SYSROOT}/usr/lib/aarch64-linux-gnu/
        ${TARGET_SYSROOT}/usr/local/qt5-arm64/lib
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY) 
