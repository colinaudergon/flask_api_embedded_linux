# Install script for directory: /home/colin/Documents/flask_api_embedded_linux/screenRock5b

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/opt/embedded/rootfs-rock5b/home/student/Apps/screenTest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/embedded/rootfs-rock5b/home/student/Apps/screenTest")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/opt/embedded/rootfs-rock5b/home/student/Apps/screenTest"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/embedded/rootfs-rock5b/home/student/Apps/screenTest")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/opt/embedded/rootfs-rock5b/home/student/Apps" TYPE EXECUTABLE FILES "/home/colin/Documents/flask_api_embedded_linux/screenRock5b/build/screenTest")
  if(EXISTS "$ENV{DESTDIR}/opt/embedded/rootfs-rock5b/home/student/Apps/screenTest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/embedded/rootfs-rock5b/home/student/Apps/screenTest")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/opt/embedded/rootfs-rock5b/home/student/Apps/screenTest"
         OLD_RPATH "/opt/embedded/sysroot/arm-64/ubuntu-22.04-arm64/usr/lib:/opt/embedded/sysroot/arm-64/ubuntu-22.04-arm64/usr/lib/aarch64-none-linux-gnu:/opt/embedded/sysroot/arm-64/ubuntu-22.04-arm64/usr/local/qt5-arm64/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-strip" "$ENV{DESTDIR}/opt/embedded/rootfs-rock5b/home/student/Apps/screenTest")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/colin/Documents/flask_api_embedded_linux/screenRock5b/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
