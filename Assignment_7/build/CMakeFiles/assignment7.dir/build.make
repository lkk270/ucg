# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.11.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.11.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/leekorff-korn/ucg/Assignment_7

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/leekorff-korn/ucg/Assignment_7/build

# Include any dependencies generated for this target.
include CMakeFiles/assignment7.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/assignment7.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/assignment7.dir/flags.make

CMakeFiles/assignment7.dir/src/main.cpp.o: CMakeFiles/assignment7.dir/flags.make
CMakeFiles/assignment7.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/leekorff-korn/ucg/Assignment_7/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/assignment7.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/assignment7.dir/src/main.cpp.o -c /Users/leekorff-korn/ucg/Assignment_7/src/main.cpp

CMakeFiles/assignment7.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/assignment7.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/leekorff-korn/ucg/Assignment_7/src/main.cpp > CMakeFiles/assignment7.dir/src/main.cpp.i

CMakeFiles/assignment7.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/assignment7.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/leekorff-korn/ucg/Assignment_7/src/main.cpp -o CMakeFiles/assignment7.dir/src/main.cpp.s

CMakeFiles/assignment7.dir/src/helpers.cpp.o: CMakeFiles/assignment7.dir/flags.make
CMakeFiles/assignment7.dir/src/helpers.cpp.o: ../src/helpers.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/leekorff-korn/ucg/Assignment_7/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/assignment7.dir/src/helpers.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/assignment7.dir/src/helpers.cpp.o -c /Users/leekorff-korn/ucg/Assignment_7/src/helpers.cpp

CMakeFiles/assignment7.dir/src/helpers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/assignment7.dir/src/helpers.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/leekorff-korn/ucg/Assignment_7/src/helpers.cpp > CMakeFiles/assignment7.dir/src/helpers.cpp.i

CMakeFiles/assignment7.dir/src/helpers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/assignment7.dir/src/helpers.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/leekorff-korn/ucg/Assignment_7/src/helpers.cpp -o CMakeFiles/assignment7.dir/src/helpers.cpp.s

# Object files for target assignment7
assignment7_OBJECTS = \
"CMakeFiles/assignment7.dir/src/main.cpp.o" \
"CMakeFiles/assignment7.dir/src/helpers.cpp.o"

# External object files for target assignment7
assignment7_EXTERNAL_OBJECTS =

assignment7: CMakeFiles/assignment7.dir/src/main.cpp.o
assignment7: CMakeFiles/assignment7.dir/src/helpers.cpp.o
assignment7: CMakeFiles/assignment7.dir/build.make
assignment7: glfw/src/libglfw3.a
assignment7: glad/libglad.a
assignment7: CMakeFiles/assignment7.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/leekorff-korn/ucg/Assignment_7/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable assignment7"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/assignment7.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/assignment7.dir/build: assignment7

.PHONY : CMakeFiles/assignment7.dir/build

CMakeFiles/assignment7.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/assignment7.dir/cmake_clean.cmake
.PHONY : CMakeFiles/assignment7.dir/clean

CMakeFiles/assignment7.dir/depend:
	cd /Users/leekorff-korn/ucg/Assignment_7/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/leekorff-korn/ucg/Assignment_7 /Users/leekorff-korn/ucg/Assignment_7 /Users/leekorff-korn/ucg/Assignment_7/build /Users/leekorff-korn/ucg/Assignment_7/build /Users/leekorff-korn/ucg/Assignment_7/build/CMakeFiles/assignment7.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/assignment7.dir/depend

