# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/colin/Documents/flask_api_embedded_linux/inputReading

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/colin/Documents/flask_api_embedded_linux/inputReading/buid

# Include any dependencies generated for this target.
include CMakeFiles/TEST_LINUX.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TEST_LINUX.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TEST_LINUX.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TEST_LINUX.dir/flags.make

CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.o: CMakeFiles/TEST_LINUX.dir/flags.make
CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.o: /home/colin/Documents/flask_api_embedded_linux/inputReading/src/gpioCtrl.c
CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.o: CMakeFiles/TEST_LINUX.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/colin/Documents/flask_api_embedded_linux/inputReading/buid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.o"
	/opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.o -MF CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.o.d -o CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.o -c /home/colin/Documents/flask_api_embedded_linux/inputReading/src/gpioCtrl.c

CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.i"
	/opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/colin/Documents/flask_api_embedded_linux/inputReading/src/gpioCtrl.c > CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.i

CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.s"
	/opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/colin/Documents/flask_api_embedded_linux/inputReading/src/gpioCtrl.c -o CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.s

CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.o: CMakeFiles/TEST_LINUX.dir/flags.make
CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.o: /home/colin/Documents/flask_api_embedded_linux/inputReading/src/gpiolib.c
CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.o: CMakeFiles/TEST_LINUX.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/colin/Documents/flask_api_embedded_linux/inputReading/buid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.o"
	/opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.o -MF CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.o.d -o CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.o -c /home/colin/Documents/flask_api_embedded_linux/inputReading/src/gpiolib.c

CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.i"
	/opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/colin/Documents/flask_api_embedded_linux/inputReading/src/gpiolib.c > CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.i

CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.s"
	/opt/embedded/crosstools/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/colin/Documents/flask_api_embedded_linux/inputReading/src/gpiolib.c -o CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.s

# Object files for target TEST_LINUX
TEST_LINUX_OBJECTS = \
"CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.o" \
"CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.o"

# External object files for target TEST_LINUX
TEST_LINUX_EXTERNAL_OBJECTS =

TEST_LINUX: CMakeFiles/TEST_LINUX.dir/src/gpioCtrl.c.o
TEST_LINUX: CMakeFiles/TEST_LINUX.dir/src/gpiolib.c.o
TEST_LINUX: CMakeFiles/TEST_LINUX.dir/build.make
TEST_LINUX: /home/colin/Documents/flask_api_embedded_linux/inputReading/lib/libperiphery.a
TEST_LINUX: CMakeFiles/TEST_LINUX.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/colin/Documents/flask_api_embedded_linux/inputReading/buid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable TEST_LINUX"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TEST_LINUX.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TEST_LINUX.dir/build: TEST_LINUX
.PHONY : CMakeFiles/TEST_LINUX.dir/build

CMakeFiles/TEST_LINUX.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TEST_LINUX.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TEST_LINUX.dir/clean

CMakeFiles/TEST_LINUX.dir/depend:
	cd /home/colin/Documents/flask_api_embedded_linux/inputReading/buid && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/colin/Documents/flask_api_embedded_linux/inputReading /home/colin/Documents/flask_api_embedded_linux/inputReading /home/colin/Documents/flask_api_embedded_linux/inputReading/buid /home/colin/Documents/flask_api_embedded_linux/inputReading/buid /home/colin/Documents/flask_api_embedded_linux/inputReading/buid/CMakeFiles/TEST_LINUX.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TEST_LINUX.dir/depend
