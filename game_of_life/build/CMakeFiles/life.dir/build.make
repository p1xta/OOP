# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/p1xta/OOP/game_of_life

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/p1xta/OOP/game_of_life/build

# Include any dependencies generated for this target.
include CMakeFiles/life.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/life.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/life.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/life.dir/flags.make

CMakeFiles/life.dir/src/main.cpp.o: CMakeFiles/life.dir/flags.make
CMakeFiles/life.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/life.dir/src/main.cpp.o: CMakeFiles/life.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/p1xta/OOP/game_of_life/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/life.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/life.dir/src/main.cpp.o -MF CMakeFiles/life.dir/src/main.cpp.o.d -o CMakeFiles/life.dir/src/main.cpp.o -c /home/p1xta/OOP/game_of_life/src/main.cpp

CMakeFiles/life.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/life.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/p1xta/OOP/game_of_life/src/main.cpp > CMakeFiles/life.dir/src/main.cpp.i

CMakeFiles/life.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/life.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/p1xta/OOP/game_of_life/src/main.cpp -o CMakeFiles/life.dir/src/main.cpp.s

CMakeFiles/life.dir/src/GameOfLife.cpp.o: CMakeFiles/life.dir/flags.make
CMakeFiles/life.dir/src/GameOfLife.cpp.o: ../src/GameOfLife.cpp
CMakeFiles/life.dir/src/GameOfLife.cpp.o: CMakeFiles/life.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/p1xta/OOP/game_of_life/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/life.dir/src/GameOfLife.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/life.dir/src/GameOfLife.cpp.o -MF CMakeFiles/life.dir/src/GameOfLife.cpp.o.d -o CMakeFiles/life.dir/src/GameOfLife.cpp.o -c /home/p1xta/OOP/game_of_life/src/GameOfLife.cpp

CMakeFiles/life.dir/src/GameOfLife.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/life.dir/src/GameOfLife.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/p1xta/OOP/game_of_life/src/GameOfLife.cpp > CMakeFiles/life.dir/src/GameOfLife.cpp.i

CMakeFiles/life.dir/src/GameOfLife.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/life.dir/src/GameOfLife.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/p1xta/OOP/game_of_life/src/GameOfLife.cpp -o CMakeFiles/life.dir/src/GameOfLife.cpp.s

# Object files for target life
life_OBJECTS = \
"CMakeFiles/life.dir/src/main.cpp.o" \
"CMakeFiles/life.dir/src/GameOfLife.cpp.o"

# External object files for target life
life_EXTERNAL_OBJECTS =

life: CMakeFiles/life.dir/src/main.cpp.o
life: CMakeFiles/life.dir/src/GameOfLife.cpp.o
life: CMakeFiles/life.dir/build.make
life: CMakeFiles/life.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/p1xta/OOP/game_of_life/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable life"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/life.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/life.dir/build: life
.PHONY : CMakeFiles/life.dir/build

CMakeFiles/life.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/life.dir/cmake_clean.cmake
.PHONY : CMakeFiles/life.dir/clean

CMakeFiles/life.dir/depend:
	cd /home/p1xta/OOP/game_of_life/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/p1xta/OOP/game_of_life /home/p1xta/OOP/game_of_life /home/p1xta/OOP/game_of_life/build /home/p1xta/OOP/game_of_life/build /home/p1xta/OOP/game_of_life/build/CMakeFiles/life.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/life.dir/depend

