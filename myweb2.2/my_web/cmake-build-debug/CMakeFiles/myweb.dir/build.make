# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kiosk/CLionProjects/my_web

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kiosk/CLionProjects/my_web/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/myweb.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/myweb.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myweb.dir/flags.make

CMakeFiles/myweb.dir/main.cpp.o: CMakeFiles/myweb.dir/flags.make
CMakeFiles/myweb.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kiosk/CLionProjects/my_web/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myweb.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myweb.dir/main.cpp.o -c /home/kiosk/CLionProjects/my_web/main.cpp

CMakeFiles/myweb.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myweb.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kiosk/CLionProjects/my_web/main.cpp > CMakeFiles/myweb.dir/main.cpp.i

CMakeFiles/myweb.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myweb.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kiosk/CLionProjects/my_web/main.cpp -o CMakeFiles/myweb.dir/main.cpp.s

CMakeFiles/myweb.dir/include/Socket.cc.o: CMakeFiles/myweb.dir/flags.make
CMakeFiles/myweb.dir/include/Socket.cc.o: ../include/Socket.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kiosk/CLionProjects/my_web/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/myweb.dir/include/Socket.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myweb.dir/include/Socket.cc.o -c /home/kiosk/CLionProjects/my_web/include/Socket.cc

CMakeFiles/myweb.dir/include/Socket.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myweb.dir/include/Socket.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kiosk/CLionProjects/my_web/include/Socket.cc > CMakeFiles/myweb.dir/include/Socket.cc.i

CMakeFiles/myweb.dir/include/Socket.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myweb.dir/include/Socket.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kiosk/CLionProjects/my_web/include/Socket.cc -o CMakeFiles/myweb.dir/include/Socket.cc.s

CMakeFiles/myweb.dir/test/add.cc.o: CMakeFiles/myweb.dir/flags.make
CMakeFiles/myweb.dir/test/add.cc.o: ../test/add.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kiosk/CLionProjects/my_web/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/myweb.dir/test/add.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myweb.dir/test/add.cc.o -c /home/kiosk/CLionProjects/my_web/test/add.cc

CMakeFiles/myweb.dir/test/add.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myweb.dir/test/add.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kiosk/CLionProjects/my_web/test/add.cc > CMakeFiles/myweb.dir/test/add.cc.i

CMakeFiles/myweb.dir/test/add.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myweb.dir/test/add.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kiosk/CLionProjects/my_web/test/add.cc -o CMakeFiles/myweb.dir/test/add.cc.s

CMakeFiles/myweb.dir/include/channel.cc.o: CMakeFiles/myweb.dir/flags.make
CMakeFiles/myweb.dir/include/channel.cc.o: ../include/channel.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kiosk/CLionProjects/my_web/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/myweb.dir/include/channel.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myweb.dir/include/channel.cc.o -c /home/kiosk/CLionProjects/my_web/include/channel.cc

CMakeFiles/myweb.dir/include/channel.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myweb.dir/include/channel.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kiosk/CLionProjects/my_web/include/channel.cc > CMakeFiles/myweb.dir/include/channel.cc.i

CMakeFiles/myweb.dir/include/channel.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myweb.dir/include/channel.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kiosk/CLionProjects/my_web/include/channel.cc -o CMakeFiles/myweb.dir/include/channel.cc.s

CMakeFiles/myweb.dir/include/Acceptor.cc.o: CMakeFiles/myweb.dir/flags.make
CMakeFiles/myweb.dir/include/Acceptor.cc.o: ../include/Acceptor.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kiosk/CLionProjects/my_web/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/myweb.dir/include/Acceptor.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myweb.dir/include/Acceptor.cc.o -c /home/kiosk/CLionProjects/my_web/include/Acceptor.cc

CMakeFiles/myweb.dir/include/Acceptor.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myweb.dir/include/Acceptor.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kiosk/CLionProjects/my_web/include/Acceptor.cc > CMakeFiles/myweb.dir/include/Acceptor.cc.i

CMakeFiles/myweb.dir/include/Acceptor.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myweb.dir/include/Acceptor.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kiosk/CLionProjects/my_web/include/Acceptor.cc -o CMakeFiles/myweb.dir/include/Acceptor.cc.s

CMakeFiles/myweb.dir/include/httpserver.cc.o: CMakeFiles/myweb.dir/flags.make
CMakeFiles/myweb.dir/include/httpserver.cc.o: ../include/httpserver.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kiosk/CLionProjects/my_web/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/myweb.dir/include/httpserver.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myweb.dir/include/httpserver.cc.o -c /home/kiosk/CLionProjects/my_web/include/httpserver.cc

CMakeFiles/myweb.dir/include/httpserver.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myweb.dir/include/httpserver.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kiosk/CLionProjects/my_web/include/httpserver.cc > CMakeFiles/myweb.dir/include/httpserver.cc.i

CMakeFiles/myweb.dir/include/httpserver.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myweb.dir/include/httpserver.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kiosk/CLionProjects/my_web/include/httpserver.cc -o CMakeFiles/myweb.dir/include/httpserver.cc.s

CMakeFiles/myweb.dir/http/http_msg_handler.cc.o: CMakeFiles/myweb.dir/flags.make
CMakeFiles/myweb.dir/http/http_msg_handler.cc.o: ../http/http_msg_handler.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kiosk/CLionProjects/my_web/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/myweb.dir/http/http_msg_handler.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myweb.dir/http/http_msg_handler.cc.o -c /home/kiosk/CLionProjects/my_web/http/http_msg_handler.cc

CMakeFiles/myweb.dir/http/http_msg_handler.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myweb.dir/http/http_msg_handler.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kiosk/CLionProjects/my_web/http/http_msg_handler.cc > CMakeFiles/myweb.dir/http/http_msg_handler.cc.i

CMakeFiles/myweb.dir/http/http_msg_handler.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myweb.dir/http/http_msg_handler.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kiosk/CLionProjects/my_web/http/http_msg_handler.cc -o CMakeFiles/myweb.dir/http/http_msg_handler.cc.s

# Object files for target myweb
myweb_OBJECTS = \
"CMakeFiles/myweb.dir/main.cpp.o" \
"CMakeFiles/myweb.dir/include/Socket.cc.o" \
"CMakeFiles/myweb.dir/test/add.cc.o" \
"CMakeFiles/myweb.dir/include/channel.cc.o" \
"CMakeFiles/myweb.dir/include/Acceptor.cc.o" \
"CMakeFiles/myweb.dir/include/httpserver.cc.o" \
"CMakeFiles/myweb.dir/http/http_msg_handler.cc.o"

# External object files for target myweb
myweb_EXTERNAL_OBJECTS =

myweb: CMakeFiles/myweb.dir/main.cpp.o
myweb: CMakeFiles/myweb.dir/include/Socket.cc.o
myweb: CMakeFiles/myweb.dir/test/add.cc.o
myweb: CMakeFiles/myweb.dir/include/channel.cc.o
myweb: CMakeFiles/myweb.dir/include/Acceptor.cc.o
myweb: CMakeFiles/myweb.dir/include/httpserver.cc.o
myweb: CMakeFiles/myweb.dir/http/http_msg_handler.cc.o
myweb: CMakeFiles/myweb.dir/build.make
myweb: CMakeFiles/myweb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kiosk/CLionProjects/my_web/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable myweb"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myweb.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myweb.dir/build: myweb

.PHONY : CMakeFiles/myweb.dir/build

CMakeFiles/myweb.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myweb.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myweb.dir/clean

CMakeFiles/myweb.dir/depend:
	cd /home/kiosk/CLionProjects/my_web/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kiosk/CLionProjects/my_web /home/kiosk/CLionProjects/my_web /home/kiosk/CLionProjects/my_web/cmake-build-debug /home/kiosk/CLionProjects/my_web/cmake-build-debug /home/kiosk/CLionProjects/my_web/cmake-build-debug/CMakeFiles/myweb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myweb.dir/depend

