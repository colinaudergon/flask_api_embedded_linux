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
CMAKE_SOURCE_DIR = /home/colin/Documents/exe_linux/prepTestLinux

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/colin/Documents/exe_linux/prepTestLinux/build

# Utility rule file for apidoc.

# Include any custom commands dependencies for this target.
include CMakeFiles/apidoc.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/apidoc.dir/progress.make

CMakeFiles/apidoc: build/doc

build/doc: /home/colin/Documents/exe_linux/prepTestLinux/doxyfile
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/colin/Documents/exe_linux/prepTestLinux/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating build/doc"
	cd /home/colin/Documents/exe_linux/prepTestLinux && /usr/bin/cmake -E echo_append Building\ API\ Documentation...
	cd /home/colin/Documents/exe_linux/prepTestLinux && /usr/bin/doxygen doxyfile
	cd /home/colin/Documents/exe_linux/prepTestLinux && /usr/bin/cmake -E echo Done.

apidoc: CMakeFiles/apidoc
apidoc: build/doc
apidoc: CMakeFiles/apidoc.dir/build.make
.PHONY : apidoc

# Rule to build all files generated by this target.
CMakeFiles/apidoc.dir/build: apidoc
.PHONY : CMakeFiles/apidoc.dir/build

CMakeFiles/apidoc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/apidoc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/apidoc.dir/clean

CMakeFiles/apidoc.dir/depend:
	cd /home/colin/Documents/exe_linux/prepTestLinux/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/colin/Documents/exe_linux/prepTestLinux /home/colin/Documents/exe_linux/prepTestLinux /home/colin/Documents/exe_linux/prepTestLinux/build /home/colin/Documents/exe_linux/prepTestLinux/build /home/colin/Documents/exe_linux/prepTestLinux/build/CMakeFiles/apidoc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/apidoc.dir/depend

