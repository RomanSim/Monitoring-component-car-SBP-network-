# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Admin\interface

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Admin\interface\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\interface.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\interface.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\interface.dir\flags.make

CMakeFiles\interface.dir\exercise.c.obj: CMakeFiles\interface.dir\flags.make
CMakeFiles\interface.dir\exercise.c.obj: ..\exercise.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Admin\interface\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/interface.dir/exercise.c.obj"
	C:\PROGRA~2\MIB055~1\2017\BUILDT~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\interface.dir\exercise.c.obj /FdCMakeFiles\interface.dir\ /FS -c C:\Users\Admin\interface\exercise.c
<<

CMakeFiles\interface.dir\exercise.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/interface.dir/exercise.c.i"
	C:\PROGRA~2\MIB055~1\2017\BUILDT~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe > CMakeFiles\interface.dir\exercise.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Admin\interface\exercise.c
<<

CMakeFiles\interface.dir\exercise.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/interface.dir/exercise.c.s"
	C:\PROGRA~2\MIB055~1\2017\BUILDT~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\interface.dir\exercise.c.s /c C:\Users\Admin\interface\exercise.c
<<

# Object files for target interface
interface_OBJECTS = \
"CMakeFiles\interface.dir\exercise.c.obj"

# External object files for target interface
interface_EXTERNAL_OBJECTS =

interface.exe: CMakeFiles\interface.dir\exercise.c.obj
interface.exe: CMakeFiles\interface.dir\build.make
interface.exe: CMakeFiles\interface.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Admin\interface\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable interface.exe"
	"C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\interface.dir --manifests  -- C:\PROGRA~2\MIB055~1\2017\BUILDT~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\interface.dir\objects1.rsp @<<
 /out:interface.exe /implib:interface.lib /pdb:C:\Users\Admin\interface\cmake-build-debug\interface.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\interface.dir\build: interface.exe

.PHONY : CMakeFiles\interface.dir\build

CMakeFiles\interface.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\interface.dir\cmake_clean.cmake
.PHONY : CMakeFiles\interface.dir\clean

CMakeFiles\interface.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\Admin\interface C:\Users\Admin\interface C:\Users\Admin\interface\cmake-build-debug C:\Users\Admin\interface\cmake-build-debug C:\Users\Admin\interface\cmake-build-debug\CMakeFiles\interface.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\interface.dir\depend
