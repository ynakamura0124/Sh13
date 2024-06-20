# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/ynakamura/cloversim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ynakamura/cloversim

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ynakamura/cloversim/CMakeFiles /home/ynakamura/cloversim/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ynakamura/cloversim/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named CloverSim

# Build rule for target.
CloverSim: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 CloverSim
.PHONY : CloverSim

# fast build rule for target.
CloverSim/fast:
	$(MAKE) -f CMakeFiles/CloverSim.dir/build.make CMakeFiles/CloverSim.dir/build
.PHONY : CloverSim/fast

#=============================================================================
# Target rules for targets named PhysicsListLib

# Build rule for target.
PhysicsListLib: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 PhysicsListLib
.PHONY : PhysicsListLib

# fast build rule for target.
PhysicsListLib/fast:
	$(MAKE) -f PhysicsList/source/CMakeFiles/PhysicsListLib.dir/build.make PhysicsList/source/CMakeFiles/PhysicsListLib.dir/build
.PHONY : PhysicsListLib/fast

#=============================================================================
# Target rules for targets named LENSLongLib

# Build rule for target.
LENSLongLib: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 LENSLongLib
.PHONY : LENSLongLib

# fast build rule for target.
LENSLongLib/fast:
	$(MAKE) -f LENSLong/source/CMakeFiles/LENSLongLib.dir/build.make LENSLong/source/CMakeFiles/LENSLongLib.dir/build
.PHONY : LENSLongLib/fast

#=============================================================================
# Target rules for targets named CloverSimLib

# Build rule for target.
CloverSimLib: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 CloverSimLib
.PHONY : CloverSimLib

# fast build rule for target.
CloverSimLib/fast:
	$(MAKE) -f source/CMakeFiles/CloverSimLib.dir/build.make source/CMakeFiles/CloverSimLib.dir/build
.PHONY : CloverSimLib/fast

CloverSim.o: CloverSim.cpp.o

.PHONY : CloverSim.o

# target to build an object file
CloverSim.cpp.o:
	$(MAKE) -f CMakeFiles/CloverSim.dir/build.make CMakeFiles/CloverSim.dir/CloverSim.cpp.o
.PHONY : CloverSim.cpp.o

CloverSim.i: CloverSim.cpp.i

.PHONY : CloverSim.i

# target to preprocess a source file
CloverSim.cpp.i:
	$(MAKE) -f CMakeFiles/CloverSim.dir/build.make CMakeFiles/CloverSim.dir/CloverSim.cpp.i
.PHONY : CloverSim.cpp.i

CloverSim.s: CloverSim.cpp.s

.PHONY : CloverSim.s

# target to generate assembly for a file
CloverSim.cpp.s:
	$(MAKE) -f CMakeFiles/CloverSim.dir/build.make CMakeFiles/CloverSim.dir/CloverSim.cpp.s
.PHONY : CloverSim.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... install/strip"
	@echo "... install/local"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... CloverSim"
	@echo "... PhysicsListLib"
	@echo "... LENSLongLib"
	@echo "... CloverSimLib"
	@echo "... CloverSim.o"
	@echo "... CloverSim.i"
	@echo "... CloverSim.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

