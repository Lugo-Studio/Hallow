# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = C:\Users\galex\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\203.7717.62\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\galex\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\203.7717.62\bin\cmake\win\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\dev\vulkan\Hallow\project_a

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\dev\vulkan\Hallow\project_a\cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles\project_a.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\project_a.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\project_a.dir\flags.make

CMakeFiles\project_a.dir\src\application\HallowApp.cpp.obj: CMakeFiles\project_a.dir\flags.make
CMakeFiles\project_a.dir\src\application\HallowApp.cpp.obj: ..\src\application\HallowApp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\dev\vulkan\Hallow\project_a\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/project_a.dir/src/application/HallowApp.cpp.obj"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\project_a.dir\src\application\HallowApp.cpp.obj /FdCMakeFiles\project_a.dir\ /FS -c C:\dev\vulkan\Hallow\project_a\src\application\HallowApp.cpp
<<

CMakeFiles\project_a.dir\src\application\HallowApp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project_a.dir/src/application/HallowApp.cpp.i"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe > CMakeFiles\project_a.dir\src\application\HallowApp.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\dev\vulkan\Hallow\project_a\src\application\HallowApp.cpp
<<

CMakeFiles\project_a.dir\src\application\HallowApp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project_a.dir/src/application/HallowApp.cpp.s"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\project_a.dir\src\application\HallowApp.cpp.s /c C:\dev\vulkan\Hallow\project_a\src\application\HallowApp.cpp
<<

CMakeFiles\project_a.dir\src\engine\pipeline\HallowPipeline.cpp.obj: CMakeFiles\project_a.dir\flags.make
CMakeFiles\project_a.dir\src\engine\pipeline\HallowPipeline.cpp.obj: ..\src\engine\pipeline\HallowPipeline.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\dev\vulkan\Hallow\project_a\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/project_a.dir/src/engine/pipeline/HallowPipeline.cpp.obj"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\project_a.dir\src\engine\pipeline\HallowPipeline.cpp.obj /FdCMakeFiles\project_a.dir\ /FS -c C:\dev\vulkan\Hallow\project_a\src\engine\pipeline\HallowPipeline.cpp
<<

CMakeFiles\project_a.dir\src\engine\pipeline\HallowPipeline.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project_a.dir/src/engine/pipeline/HallowPipeline.cpp.i"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe > CMakeFiles\project_a.dir\src\engine\pipeline\HallowPipeline.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\dev\vulkan\Hallow\project_a\src\engine\pipeline\HallowPipeline.cpp
<<

CMakeFiles\project_a.dir\src\engine\pipeline\HallowPipeline.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project_a.dir/src/engine/pipeline/HallowPipeline.cpp.s"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\project_a.dir\src\engine\pipeline\HallowPipeline.cpp.s /c C:\dev\vulkan\Hallow\project_a\src\engine\pipeline\HallowPipeline.cpp
<<

CMakeFiles\project_a.dir\src\engine\time\Time.cpp.obj: CMakeFiles\project_a.dir\flags.make
CMakeFiles\project_a.dir\src\engine\time\Time.cpp.obj: ..\src\engine\time\Time.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\dev\vulkan\Hallow\project_a\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/project_a.dir/src/engine/time/Time.cpp.obj"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\project_a.dir\src\engine\time\Time.cpp.obj /FdCMakeFiles\project_a.dir\ /FS -c C:\dev\vulkan\Hallow\project_a\src\engine\time\Time.cpp
<<

CMakeFiles\project_a.dir\src\engine\time\Time.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project_a.dir/src/engine/time/Time.cpp.i"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe > CMakeFiles\project_a.dir\src\engine\time\Time.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\dev\vulkan\Hallow\project_a\src\engine\time\Time.cpp
<<

CMakeFiles\project_a.dir\src\engine\time\Time.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project_a.dir/src/engine/time/Time.cpp.s"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\project_a.dir\src\engine\time\Time.cpp.s /c C:\dev\vulkan\Hallow\project_a\src\engine\time\Time.cpp
<<

CMakeFiles\project_a.dir\src\engine\window\HallowWindow.cpp.obj: CMakeFiles\project_a.dir\flags.make
CMakeFiles\project_a.dir\src\engine\window\HallowWindow.cpp.obj: ..\src\engine\window\HallowWindow.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\dev\vulkan\Hallow\project_a\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/project_a.dir/src/engine/window/HallowWindow.cpp.obj"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\project_a.dir\src\engine\window\HallowWindow.cpp.obj /FdCMakeFiles\project_a.dir\ /FS -c C:\dev\vulkan\Hallow\project_a\src\engine\window\HallowWindow.cpp
<<

CMakeFiles\project_a.dir\src\engine\window\HallowWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project_a.dir/src/engine/window/HallowWindow.cpp.i"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe > CMakeFiles\project_a.dir\src\engine\window\HallowWindow.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\dev\vulkan\Hallow\project_a\src\engine\window\HallowWindow.cpp
<<

CMakeFiles\project_a.dir\src\engine\window\HallowWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project_a.dir/src/engine/window/HallowWindow.cpp.s"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\project_a.dir\src\engine\window\HallowWindow.cpp.s /c C:\dev\vulkan\Hallow\project_a\src\engine\window\HallowWindow.cpp
<<

CMakeFiles\project_a.dir\src\main.cpp.obj: CMakeFiles\project_a.dir\flags.make
CMakeFiles\project_a.dir\src\main.cpp.obj: ..\src\main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\dev\vulkan\Hallow\project_a\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/project_a.dir/src/main.cpp.obj"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\project_a.dir\src\main.cpp.obj /FdCMakeFiles\project_a.dir\ /FS -c C:\dev\vulkan\Hallow\project_a\src\main.cpp
<<

CMakeFiles\project_a.dir\src\main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project_a.dir/src/main.cpp.i"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe > CMakeFiles\project_a.dir\src\main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\dev\vulkan\Hallow\project_a\src\main.cpp
<<

CMakeFiles\project_a.dir\src\main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project_a.dir/src/main.cpp.s"
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\project_a.dir\src\main.cpp.s /c C:\dev\vulkan\Hallow\project_a\src\main.cpp
<<

# Object files for target project_a
project_a_OBJECTS = \
"CMakeFiles\project_a.dir\src\application\HallowApp.cpp.obj" \
"CMakeFiles\project_a.dir\src\engine\pipeline\HallowPipeline.cpp.obj" \
"CMakeFiles\project_a.dir\src\engine\time\Time.cpp.obj" \
"CMakeFiles\project_a.dir\src\engine\window\HallowWindow.cpp.obj" \
"CMakeFiles\project_a.dir\src\main.cpp.obj"

# External object files for target project_a
project_a_EXTERNAL_OBJECTS =

project_a.exe: CMakeFiles\project_a.dir\src\application\HallowApp.cpp.obj
project_a.exe: CMakeFiles\project_a.dir\src\engine\pipeline\HallowPipeline.cpp.obj
project_a.exe: CMakeFiles\project_a.dir\src\engine\time\Time.cpp.obj
project_a.exe: CMakeFiles\project_a.dir\src\engine\window\HallowWindow.cpp.obj
project_a.exe: CMakeFiles\project_a.dir\src\main.cpp.obj
project_a.exe: CMakeFiles\project_a.dir\build.make
project_a.exe: C:\lib\GLFW\glfw-3.3.3.bin.WIN64\lib\glfw3.lib
project_a.exe: C:\VulkanSDK\1.2.170.0\Lib\vulkan-1.lib
project_a.exe: CMakeFiles\project_a.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\dev\vulkan\Hallow\project_a\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable project_a.exe"
	C:\Users\galex\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\203.7717.62\bin\cmake\win\bin\cmake.exe -E vs_link_exe --intdir=CMakeFiles\project_a.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x64\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x64\mt.exe --manifests  -- C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1425~1.286\bin\Hostx64\x64\link.exe /nologo @CMakeFiles\project_a.dir\objects1.rsp @<<
 /out:project_a.exe /implib:project_a.lib /pdb:C:\dev\vulkan\Hallow\project_a\cmake-build-release\project_a.pdb /version:0.0  /machine:x64 /INCREMENTAL:NO /subsystem:console   -LIBPATH:C:\dev\vulkan\Hallow\project_a\lib  C:\lib\GLFW\glfw-3.3.3.bin.WIN64\lib\glfw3.lib C:\VulkanSDK\1.2.170.0\Lib\vulkan-1.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\project_a.dir\build: project_a.exe

.PHONY : CMakeFiles\project_a.dir\build

CMakeFiles\project_a.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\project_a.dir\cmake_clean.cmake
.PHONY : CMakeFiles\project_a.dir\clean

CMakeFiles\project_a.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\dev\vulkan\Hallow\project_a C:\dev\vulkan\Hallow\project_a C:\dev\vulkan\Hallow\project_a\cmake-build-release C:\dev\vulkan\Hallow\project_a\cmake-build-release C:\dev\vulkan\Hallow\project_a\cmake-build-release\CMakeFiles\project_a.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\project_a.dir\depend

