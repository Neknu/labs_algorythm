# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\University\course2\labs_algorythm\lab9_sem2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\University\course2\labs_algorythm\lab9_sem2\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lab9_sem2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lab9_sem2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab9_sem2.dir/flags.make

CMakeFiles/lab9_sem2.dir/main.cpp.obj: CMakeFiles/lab9_sem2.dir/flags.make
CMakeFiles/lab9_sem2.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\University\course2\labs_algorythm\lab9_sem2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lab9_sem2.dir/main.cpp.obj"
	C:\PROGRA~1\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lab9_sem2.dir\main.cpp.obj -c D:\University\course2\labs_algorythm\lab9_sem2\main.cpp

CMakeFiles/lab9_sem2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab9_sem2.dir/main.cpp.i"
	C:\PROGRA~1\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\University\course2\labs_algorythm\lab9_sem2\main.cpp > CMakeFiles\lab9_sem2.dir\main.cpp.i

CMakeFiles/lab9_sem2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab9_sem2.dir/main.cpp.s"
	C:\PROGRA~1\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\University\course2\labs_algorythm\lab9_sem2\main.cpp -o CMakeFiles\lab9_sem2.dir\main.cpp.s

CMakeFiles/lab9_sem2.dir/sqlite3.c.obj: CMakeFiles/lab9_sem2.dir/flags.make
CMakeFiles/lab9_sem2.dir/sqlite3.c.obj: ../sqlite3.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\University\course2\labs_algorythm\lab9_sem2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/lab9_sem2.dir/sqlite3.c.obj"
	C:\PROGRA~1\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\lab9_sem2.dir\sqlite3.c.obj   -c D:\University\course2\labs_algorythm\lab9_sem2\sqlite3.c

CMakeFiles/lab9_sem2.dir/sqlite3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab9_sem2.dir/sqlite3.c.i"
	C:\PROGRA~1\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\University\course2\labs_algorythm\lab9_sem2\sqlite3.c > CMakeFiles\lab9_sem2.dir\sqlite3.c.i

CMakeFiles/lab9_sem2.dir/sqlite3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab9_sem2.dir/sqlite3.c.s"
	C:\PROGRA~1\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\University\course2\labs_algorythm\lab9_sem2\sqlite3.c -o CMakeFiles\lab9_sem2.dir\sqlite3.c.s

# Object files for target lab9_sem2
lab9_sem2_OBJECTS = \
"CMakeFiles/lab9_sem2.dir/main.cpp.obj" \
"CMakeFiles/lab9_sem2.dir/sqlite3.c.obj"

# External object files for target lab9_sem2
lab9_sem2_EXTERNAL_OBJECTS =

lab9_sem2.exe: CMakeFiles/lab9_sem2.dir/main.cpp.obj
lab9_sem2.exe: CMakeFiles/lab9_sem2.dir/sqlite3.c.obj
lab9_sem2.exe: CMakeFiles/lab9_sem2.dir/build.make
lab9_sem2.exe: CMakeFiles/lab9_sem2.dir/linklibs.rsp
lab9_sem2.exe: CMakeFiles/lab9_sem2.dir/objects1.rsp
lab9_sem2.exe: CMakeFiles/lab9_sem2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\University\course2\labs_algorythm\lab9_sem2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable lab9_sem2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lab9_sem2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab9_sem2.dir/build: lab9_sem2.exe

.PHONY : CMakeFiles/lab9_sem2.dir/build

CMakeFiles/lab9_sem2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lab9_sem2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/lab9_sem2.dir/clean

CMakeFiles/lab9_sem2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\University\course2\labs_algorythm\lab9_sem2 D:\University\course2\labs_algorythm\lab9_sem2 D:\University\course2\labs_algorythm\lab9_sem2\cmake-build-debug D:\University\course2\labs_algorythm\lab9_sem2\cmake-build-debug D:\University\course2\labs_algorythm\lab9_sem2\cmake-build-debug\CMakeFiles\lab9_sem2.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lab9_sem2.dir/depend

