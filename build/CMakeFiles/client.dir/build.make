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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Projects/Appetito

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Projects/Appetito/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/src/client.cpp.obj: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/client.cpp.obj: CMakeFiles/client.dir/includes_CXX.rsp
CMakeFiles/client.dir/src/client.cpp.obj: C:/Projects/Appetito/src/client.cpp
CMakeFiles/client.dir/src/client.cpp.obj: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Projects/Appetito/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/src/client.cpp.obj"
	C:/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/client.cpp.obj -MF CMakeFiles/client.dir/src/client.cpp.obj.d -o CMakeFiles/client.dir/src/client.cpp.obj -c C:/Projects/Appetito/src/client.cpp

CMakeFiles/client.dir/src/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/client.cpp.i"
	C:/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Projects/Appetito/src/client.cpp > CMakeFiles/client.dir/src/client.cpp.i

CMakeFiles/client.dir/src/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/client.cpp.s"
	C:/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Projects/Appetito/src/client.cpp -o CMakeFiles/client.dir/src/client.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/src/client.cpp.obj"

# External object files for target client
client_EXTERNAL_OBJECTS =

bin/client.exe: CMakeFiles/client.dir/src/client.cpp.obj
bin/client.exe: CMakeFiles/client.dir/build.make
bin/client.exe: CMakeFiles/client.dir/linkLibs.rsp
bin/client.exe: CMakeFiles/client.dir/objects1
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:/Projects/Appetito/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/client.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/client.dir/objects.a
	C:/msys64/mingw64/bin/ar.exe qc CMakeFiles/client.dir/objects.a @CMakeFiles/client.dir/objects1
	C:/msys64/mingw64/bin/g++.exe -g -Wl,--whole-archive CMakeFiles/client.dir/objects.a -Wl,--no-whole-archive -o bin/client.exe -Wl,--out-implib,libclient.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/client.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: bin/client.exe
.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Projects/Appetito C:/Projects/Appetito C:/Projects/Appetito/build C:/Projects/Appetito/build C:/Projects/Appetito/build/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

