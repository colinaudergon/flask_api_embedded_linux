set(CMAKE_SYSTEM_NAME               Linux)
set(CMAKE_SYSTEM_PROCESSOR          arm32)

# Without this flag CMake is not able to pass the compiler sanity check.
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

get_filename_component(TOOLCHAIN_ROOT "${LINUX_ARM_TOOLCHAIN_PATH}" DIRECTORY)
set(CMAKE_C_FLAGS                   "${APP_C_FLAGS} -target arm-linux-gnueabihf --gcc-toolchain=${TOOLCHAIN_ROOT} --sysroot=${TOOLCHAIN_ROOT}/arm-none-linux-gnueabihf/libc" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS                 "${APP_CXX_FLAGS} ${CMAKE_C_FLAGS}" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG             "-O0 -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "-O3 -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS          "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=${LINUX_ARM_TOOLCHAIN_PATH}arm-none-linux-gnueabihf-ld")

# Set the rootfs and sysroot path
set(TARGET_ROOTFS /opt/embedded/stm32mp1/rootfs)
set(TARGET_SYSROOT /opt/embedded/sysroot/arm-32/ubuntu-22.04-armhf)
include_directories(${TARGET_SYSROOT}/usr/include ${TARGET_SYSROOT}/usr/local/include ${TARGET_SYSROOT}/usr/include/arm-linux-gnueabihf ${TARGET_SYSROOT}/usr/include/drm)

# Sysroot library paths
link_directories(
        ${TARGRT_SYSROOT}/lib
        ${TARGET_SYSROOT}/usr/lib
        ${TARGET_SYSROOT}/usr/lib/arm-none-linux-gnueabihf
        ${TARGET_SYSROOT}/usr/local/qt5-arm32/lib
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY) 
