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
CMAKE_SOURCE_DIR = /root/code/CzyNetFrame/CzyNetFrame

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/code/CzyNetFrame/CzyNetFrame/build

# Include any dependencies generated for this target.
include src/base/CMakeFiles/base_shared.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/base/CMakeFiles/base_shared.dir/compiler_depend.make

# Include the progress variables for this target.
include src/base/CMakeFiles/base_shared.dir/progress.make

# Include the compile flags for this target's objects.
include src/base/CMakeFiles/base_shared.dir/flags.make

src/base/CMakeFiles/base_shared.dir/CurrentThread.cc.o: src/base/CMakeFiles/base_shared.dir/flags.make
src/base/CMakeFiles/base_shared.dir/CurrentThread.cc.o: ../src/base/CurrentThread.cc
src/base/CMakeFiles/base_shared.dir/CurrentThread.cc.o: src/base/CMakeFiles/base_shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/code/CzyNetFrame/CzyNetFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/base/CMakeFiles/base_shared.dir/CurrentThread.cc.o"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/base_shared.dir/CurrentThread.cc.o -MF CMakeFiles/base_shared.dir/CurrentThread.cc.o.d -o CMakeFiles/base_shared.dir/CurrentThread.cc.o -c /root/code/CzyNetFrame/CzyNetFrame/src/base/CurrentThread.cc

src/base/CMakeFiles/base_shared.dir/CurrentThread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base_shared.dir/CurrentThread.cc.i"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/code/CzyNetFrame/CzyNetFrame/src/base/CurrentThread.cc > CMakeFiles/base_shared.dir/CurrentThread.cc.i

src/base/CMakeFiles/base_shared.dir/CurrentThread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base_shared.dir/CurrentThread.cc.s"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/code/CzyNetFrame/CzyNetFrame/src/base/CurrentThread.cc -o CMakeFiles/base_shared.dir/CurrentThread.cc.s

src/base/CMakeFiles/base_shared.dir/Date.cc.o: src/base/CMakeFiles/base_shared.dir/flags.make
src/base/CMakeFiles/base_shared.dir/Date.cc.o: ../src/base/Date.cc
src/base/CMakeFiles/base_shared.dir/Date.cc.o: src/base/CMakeFiles/base_shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/code/CzyNetFrame/CzyNetFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/base/CMakeFiles/base_shared.dir/Date.cc.o"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/base_shared.dir/Date.cc.o -MF CMakeFiles/base_shared.dir/Date.cc.o.d -o CMakeFiles/base_shared.dir/Date.cc.o -c /root/code/CzyNetFrame/CzyNetFrame/src/base/Date.cc

src/base/CMakeFiles/base_shared.dir/Date.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base_shared.dir/Date.cc.i"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/code/CzyNetFrame/CzyNetFrame/src/base/Date.cc > CMakeFiles/base_shared.dir/Date.cc.i

src/base/CMakeFiles/base_shared.dir/Date.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base_shared.dir/Date.cc.s"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/code/CzyNetFrame/CzyNetFrame/src/base/Date.cc -o CMakeFiles/base_shared.dir/Date.cc.s

src/base/CMakeFiles/base_shared.dir/Log.cc.o: src/base/CMakeFiles/base_shared.dir/flags.make
src/base/CMakeFiles/base_shared.dir/Log.cc.o: ../src/base/Log.cc
src/base/CMakeFiles/base_shared.dir/Log.cc.o: src/base/CMakeFiles/base_shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/code/CzyNetFrame/CzyNetFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/base/CMakeFiles/base_shared.dir/Log.cc.o"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/base_shared.dir/Log.cc.o -MF CMakeFiles/base_shared.dir/Log.cc.o.d -o CMakeFiles/base_shared.dir/Log.cc.o -c /root/code/CzyNetFrame/CzyNetFrame/src/base/Log.cc

src/base/CMakeFiles/base_shared.dir/Log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base_shared.dir/Log.cc.i"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/code/CzyNetFrame/CzyNetFrame/src/base/Log.cc > CMakeFiles/base_shared.dir/Log.cc.i

src/base/CMakeFiles/base_shared.dir/Log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base_shared.dir/Log.cc.s"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/code/CzyNetFrame/CzyNetFrame/src/base/Log.cc -o CMakeFiles/base_shared.dir/Log.cc.s

src/base/CMakeFiles/base_shared.dir/LogStream.cc.o: src/base/CMakeFiles/base_shared.dir/flags.make
src/base/CMakeFiles/base_shared.dir/LogStream.cc.o: ../src/base/LogStream.cc
src/base/CMakeFiles/base_shared.dir/LogStream.cc.o: src/base/CMakeFiles/base_shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/code/CzyNetFrame/CzyNetFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/base/CMakeFiles/base_shared.dir/LogStream.cc.o"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/base_shared.dir/LogStream.cc.o -MF CMakeFiles/base_shared.dir/LogStream.cc.o.d -o CMakeFiles/base_shared.dir/LogStream.cc.o -c /root/code/CzyNetFrame/CzyNetFrame/src/base/LogStream.cc

src/base/CMakeFiles/base_shared.dir/LogStream.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base_shared.dir/LogStream.cc.i"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/code/CzyNetFrame/CzyNetFrame/src/base/LogStream.cc > CMakeFiles/base_shared.dir/LogStream.cc.i

src/base/CMakeFiles/base_shared.dir/LogStream.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base_shared.dir/LogStream.cc.s"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/code/CzyNetFrame/CzyNetFrame/src/base/LogStream.cc -o CMakeFiles/base_shared.dir/LogStream.cc.s

src/base/CMakeFiles/base_shared.dir/LstTimer.cc.o: src/base/CMakeFiles/base_shared.dir/flags.make
src/base/CMakeFiles/base_shared.dir/LstTimer.cc.o: ../src/base/LstTimer.cc
src/base/CMakeFiles/base_shared.dir/LstTimer.cc.o: src/base/CMakeFiles/base_shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/code/CzyNetFrame/CzyNetFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/base/CMakeFiles/base_shared.dir/LstTimer.cc.o"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/base_shared.dir/LstTimer.cc.o -MF CMakeFiles/base_shared.dir/LstTimer.cc.o.d -o CMakeFiles/base_shared.dir/LstTimer.cc.o -c /root/code/CzyNetFrame/CzyNetFrame/src/base/LstTimer.cc

src/base/CMakeFiles/base_shared.dir/LstTimer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base_shared.dir/LstTimer.cc.i"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/code/CzyNetFrame/CzyNetFrame/src/base/LstTimer.cc > CMakeFiles/base_shared.dir/LstTimer.cc.i

src/base/CMakeFiles/base_shared.dir/LstTimer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base_shared.dir/LstTimer.cc.s"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/code/CzyNetFrame/CzyNetFrame/src/base/LstTimer.cc -o CMakeFiles/base_shared.dir/LstTimer.cc.s

src/base/CMakeFiles/base_shared.dir/TimeStamp.cc.o: src/base/CMakeFiles/base_shared.dir/flags.make
src/base/CMakeFiles/base_shared.dir/TimeStamp.cc.o: ../src/base/TimeStamp.cc
src/base/CMakeFiles/base_shared.dir/TimeStamp.cc.o: src/base/CMakeFiles/base_shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/code/CzyNetFrame/CzyNetFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/base/CMakeFiles/base_shared.dir/TimeStamp.cc.o"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/base_shared.dir/TimeStamp.cc.o -MF CMakeFiles/base_shared.dir/TimeStamp.cc.o.d -o CMakeFiles/base_shared.dir/TimeStamp.cc.o -c /root/code/CzyNetFrame/CzyNetFrame/src/base/TimeStamp.cc

src/base/CMakeFiles/base_shared.dir/TimeStamp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base_shared.dir/TimeStamp.cc.i"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/code/CzyNetFrame/CzyNetFrame/src/base/TimeStamp.cc > CMakeFiles/base_shared.dir/TimeStamp.cc.i

src/base/CMakeFiles/base_shared.dir/TimeStamp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base_shared.dir/TimeStamp.cc.s"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/code/CzyNetFrame/CzyNetFrame/src/base/TimeStamp.cc -o CMakeFiles/base_shared.dir/TimeStamp.cc.s

src/base/CMakeFiles/base_shared.dir/TimeZone.cc.o: src/base/CMakeFiles/base_shared.dir/flags.make
src/base/CMakeFiles/base_shared.dir/TimeZone.cc.o: ../src/base/TimeZone.cc
src/base/CMakeFiles/base_shared.dir/TimeZone.cc.o: src/base/CMakeFiles/base_shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/code/CzyNetFrame/CzyNetFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/base/CMakeFiles/base_shared.dir/TimeZone.cc.o"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/base_shared.dir/TimeZone.cc.o -MF CMakeFiles/base_shared.dir/TimeZone.cc.o.d -o CMakeFiles/base_shared.dir/TimeZone.cc.o -c /root/code/CzyNetFrame/CzyNetFrame/src/base/TimeZone.cc

src/base/CMakeFiles/base_shared.dir/TimeZone.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base_shared.dir/TimeZone.cc.i"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/code/CzyNetFrame/CzyNetFrame/src/base/TimeZone.cc > CMakeFiles/base_shared.dir/TimeZone.cc.i

src/base/CMakeFiles/base_shared.dir/TimeZone.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base_shared.dir/TimeZone.cc.s"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/code/CzyNetFrame/CzyNetFrame/src/base/TimeZone.cc -o CMakeFiles/base_shared.dir/TimeZone.cc.s

# Object files for target base_shared
base_shared_OBJECTS = \
"CMakeFiles/base_shared.dir/CurrentThread.cc.o" \
"CMakeFiles/base_shared.dir/Date.cc.o" \
"CMakeFiles/base_shared.dir/Log.cc.o" \
"CMakeFiles/base_shared.dir/LogStream.cc.o" \
"CMakeFiles/base_shared.dir/LstTimer.cc.o" \
"CMakeFiles/base_shared.dir/TimeStamp.cc.o" \
"CMakeFiles/base_shared.dir/TimeZone.cc.o"

# External object files for target base_shared
base_shared_EXTERNAL_OBJECTS =

lib/libbase_shared.so: src/base/CMakeFiles/base_shared.dir/CurrentThread.cc.o
lib/libbase_shared.so: src/base/CMakeFiles/base_shared.dir/Date.cc.o
lib/libbase_shared.so: src/base/CMakeFiles/base_shared.dir/Log.cc.o
lib/libbase_shared.so: src/base/CMakeFiles/base_shared.dir/LogStream.cc.o
lib/libbase_shared.so: src/base/CMakeFiles/base_shared.dir/LstTimer.cc.o
lib/libbase_shared.so: src/base/CMakeFiles/base_shared.dir/TimeStamp.cc.o
lib/libbase_shared.so: src/base/CMakeFiles/base_shared.dir/TimeZone.cc.o
lib/libbase_shared.so: src/base/CMakeFiles/base_shared.dir/build.make
lib/libbase_shared.so: src/base/CMakeFiles/base_shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/code/CzyNetFrame/CzyNetFrame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX shared library ../../lib/libbase_shared.so"
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/base_shared.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/base/CMakeFiles/base_shared.dir/build: lib/libbase_shared.so
.PHONY : src/base/CMakeFiles/base_shared.dir/build

src/base/CMakeFiles/base_shared.dir/clean:
	cd /root/code/CzyNetFrame/CzyNetFrame/build/src/base && $(CMAKE_COMMAND) -P CMakeFiles/base_shared.dir/cmake_clean.cmake
.PHONY : src/base/CMakeFiles/base_shared.dir/clean

src/base/CMakeFiles/base_shared.dir/depend:
	cd /root/code/CzyNetFrame/CzyNetFrame/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/code/CzyNetFrame/CzyNetFrame /root/code/CzyNetFrame/CzyNetFrame/src/base /root/code/CzyNetFrame/CzyNetFrame/build /root/code/CzyNetFrame/CzyNetFrame/build/src/base /root/code/CzyNetFrame/CzyNetFrame/build/src/base/CMakeFiles/base_shared.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/base/CMakeFiles/base_shared.dir/depend

