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
CMAKE_SOURCE_DIR = /home/colin/Documents/flask_api_embedded_linux/readingInputsProto

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/colin/Documents/flask_api_embedded_linux/readingInputsProto/build

# Utility rule file for apidoc_forced.

# Include any custom commands dependencies for this target.
include CMakeFiles/apidoc_forced.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/apidoc_forced.dir/progress.make

CMakeFiles/apidoc_forced:
	cd /home/colin/Documents/flask_api_embedded_linux/readingInputsProto && /usr/bin/cmake -E echo_append Building\ API\ Documentation...
	cd /home/colin/Documents/flask_api_embedded_linux/readingInputsProto && /usr/bin/doxygen doxyfile
	cd /home/colin/Documents/flask_api_embedded_linux/readingInputsProto && /usr/bin/cmake -E echo Done.

apidoc_forced: CMakeFiles/apidoc_forced
apidoc_forced: CMakeFiles/apidoc_forced.dir/build.make
.PHONY : apidoc_forced

# Rule to build all files generated by this target.
CMakeFiles/apidoc_forced.dir/build: apidoc_forced
.PHONY : CMakeFiles/apidoc_forced.dir/build

CMakeFiles/apidoc_forced.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/apidoc_forced.dir/cmake_clean.cmake
.PHONY : CMakeFiles/apidoc_forced.dir/clean

CMakeFiles/apidoc_forced.dir/depend:
	cd /home/colin/Documents/flask_api_embedded_linux/readingInputsProto/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/colin/Documents/flask_api_embedded_linux/readingInputsProto /home/colin/Documents/flask_api_embedded_linux/readingInputsProto /home/colin/Documents/flask_api_embedded_linux/readingInputsProto/build /home/colin/Documents/flask_api_embedded_linux/readingInputsProto/build /home/colin/Documents/flask_api_embedded_linux/readingInputsProto/build/CMakeFiles/apidoc_forced.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/apidoc_forced.dir/depend

