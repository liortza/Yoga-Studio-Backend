# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /snap/clion/169/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/169/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/spl211/CLionProjects/Assignment1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/spl211/CLionProjects/Assignment1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Assignment1.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Assignment1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Assignment1.dir/flags.make

CMakeFiles/Assignment1.dir/main.cpp.o: CMakeFiles/Assignment1.dir/flags.make
CMakeFiles/Assignment1.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/spl211/CLionProjects/Assignment1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Assignment1.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Assignment1.dir/main.cpp.o -c /home/spl211/CLionProjects/Assignment1/main.cpp

CMakeFiles/Assignment1.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Assignment1.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/spl211/CLionProjects/Assignment1/main.cpp > CMakeFiles/Assignment1.dir/main.cpp.i

CMakeFiles/Assignment1.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Assignment1.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/spl211/CLionProjects/Assignment1/main.cpp -o CMakeFiles/Assignment1.dir/main.cpp.s

# Object files for target Assignment1
Assignment1_OBJECTS = \
"CMakeFiles/Assignment1.dir/main.cpp.o"

# External object files for target Assignment1
Assignment1_EXTERNAL_OBJECTS =

Assignment1: CMakeFiles/Assignment1.dir/main.cpp.o
Assignment1: CMakeFiles/Assignment1.dir/build.make
Assignment1: CMakeFiles/Assignment1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/spl211/CLionProjects/Assignment1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Assignment1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Assignment1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Assignment1.dir/build: Assignment1
.PHONY : CMakeFiles/Assignment1.dir/build

CMakeFiles/Assignment1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Assignment1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Assignment1.dir/clean

CMakeFiles/Assignment1.dir/depend:
	cd /home/spl211/CLionProjects/Assignment1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/spl211/CLionProjects/Assignment1 /home/spl211/CLionProjects/Assignment1 /home/spl211/CLionProjects/Assignment1/cmake-build-debug /home/spl211/CLionProjects/Assignment1/cmake-build-debug /home/spl211/CLionProjects/Assignment1/cmake-build-debug/CMakeFiles/Assignment1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Assignment1.dir/depend
