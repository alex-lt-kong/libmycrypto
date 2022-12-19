# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/akong_yin/my-repos/cryptographic-algorithms

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/akong_yin/my-repos/cryptographic-algorithms/build

# Include any dependencies generated for this target.
include src/CMakeFiles/base64.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/base64.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/base64.dir/flags.make

src/CMakeFiles/base64.dir/base64.c.o: src/CMakeFiles/base64.dir/flags.make
src/CMakeFiles/base64.dir/base64.c.o: ../src/base64.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/akong_yin/my-repos/cryptographic-algorithms/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/base64.dir/base64.c.o"
	cd /home/akong_yin/my-repos/cryptographic-algorithms/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/base64.dir/base64.c.o -c /home/akong_yin/my-repos/cryptographic-algorithms/src/base64.c

src/CMakeFiles/base64.dir/base64.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/base64.dir/base64.c.i"
	cd /home/akong_yin/my-repos/cryptographic-algorithms/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/akong_yin/my-repos/cryptographic-algorithms/src/base64.c > CMakeFiles/base64.dir/base64.c.i

src/CMakeFiles/base64.dir/base64.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/base64.dir/base64.c.s"
	cd /home/akong_yin/my-repos/cryptographic-algorithms/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/akong_yin/my-repos/cryptographic-algorithms/src/base64.c -o CMakeFiles/base64.dir/base64.c.s

# Object files for target base64
base64_OBJECTS = \
"CMakeFiles/base64.dir/base64.c.o"

# External object files for target base64
base64_EXTERNAL_OBJECTS =

src/libbase64.a: src/CMakeFiles/base64.dir/base64.c.o
src/libbase64.a: src/CMakeFiles/base64.dir/build.make
src/libbase64.a: src/CMakeFiles/base64.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/akong_yin/my-repos/cryptographic-algorithms/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libbase64.a"
	cd /home/akong_yin/my-repos/cryptographic-algorithms/build/src && $(CMAKE_COMMAND) -P CMakeFiles/base64.dir/cmake_clean_target.cmake
	cd /home/akong_yin/my-repos/cryptographic-algorithms/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/base64.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/base64.dir/build: src/libbase64.a

.PHONY : src/CMakeFiles/base64.dir/build

src/CMakeFiles/base64.dir/clean:
	cd /home/akong_yin/my-repos/cryptographic-algorithms/build/src && $(CMAKE_COMMAND) -P CMakeFiles/base64.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/base64.dir/clean

src/CMakeFiles/base64.dir/depend:
	cd /home/akong_yin/my-repos/cryptographic-algorithms/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/akong_yin/my-repos/cryptographic-algorithms /home/akong_yin/my-repos/cryptographic-algorithms/src /home/akong_yin/my-repos/cryptographic-algorithms/build /home/akong_yin/my-repos/cryptographic-algorithms/build/src /home/akong_yin/my-repos/cryptographic-algorithms/build/src/CMakeFiles/base64.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/base64.dir/depend

