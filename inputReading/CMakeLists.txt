# This one is important
set(CMAKE_SYSTEM_NAME Linux)

# This one not so much
set(CMAKE_SYSTEM_VERSION 1)

# Specify the version being used as well as the language
cmake_minimum_required(VERSION 3.7)

# Name your project here
project(TEST_LINUX)

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
)

# Executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Add libraries to be linked with
if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm32")
 target_link_libraries(${CMAKE_PROJECT_NAME}
 	pthread
 	rt
 	m
 	drm
        # Add static libraries from the lib directory
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libperiphery.a
 )
 elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
 target_link_libraries(${CMAKE_PROJECT_NAME}
 	pthread
 	rt
 	m
 	drm
        # Add static libraries from the lib directory
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libperiphery.a

 )
else (CMAKE_SYSTEM_PROCESSOR MATCHES "intel")
 target_link_libraries(${CMAKE_PROJECT_NAME}
 	pthread
 	rt
 	m
 	drm
        # Add static libraries from the lib directory
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libperiphery.a

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

