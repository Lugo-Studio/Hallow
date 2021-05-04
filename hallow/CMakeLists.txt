cmake_minimum_required(VERSION 3.19)
project(hallow)

set(CMAKE_CXX_STANDARD 20)

# Directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(INCL_DIR "${CMAKE_SOURCE_DIR}/include")
set(RES_DIR "${CMAKE_SOURCE_DIR}/res")

# Add source files
file(GLOB_RECURSE SOURCE_FILES
     ${SRC_DIR}/*.c
     ${SRC_DIR}/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES
     ${SRC_DIR}/*.h
     ${SRC_DIR}/*.hpp)

# Add shader files
file(GLOB_RECURSE VERT_SHADER_FILES RELATIVE ${CMAKE_SOURCE_DIR}
     ${RES_DIR}/*.vert)
message("Vert shaders found: ${VERT_SHADER_FILES}")
file(GLOB_RECURSE FRAG_SHADER_FILES RELATIVE ${CMAKE_SOURCE_DIR}
     ${RES_DIR}/*.frag)
message("Frag shaders found: ${FRAG_SHADER_FILES}")

# Compile Shaders
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/res/shaders)
function(add_shader src)
  message(STATUS "Compiling shader: ${src}")
  find_program(GLSLC glslc)
  set(CURRENT_SHADER ${CMAKE_SOURCE_DIR}/${src})
  set(CURRENT_SHADER_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${src}.spv)
  set(GLSLC_CMD ${GLSLC} ${CURRENT_SHADER} -o ${CURRENT_SHADER_OUTPUT})
  execute_process(COMMAND ${GLSLC_CMD})
endfunction()

if (VERT_SHADER_FILES)
  message(STATUS "Compiling vertex shaders...")
  add_shader(${VERT_SHADER_FILES})
endif ()
if (FRAG_SHADER_FILES)
  message(STATUS "Compiling fragment shaders...")
  add_shader(${FRAG_SHADER_FILES})
endif ()

# Configure assets header file
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

# Hide console on release & define the executable
message(STATUS "Cmake build type: ${CMAKE_BUILD_TYPE}")
if (${CMAKE_BUILD_TYPE} STREQUAL Release)
  add_compile_definitions(MODE_RELEASE)
  add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})
else ()
  add_compile_definitions(MODE_DEBUG)
  add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
endif ()

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Vulkan
find_package(Vulkan REQUIRED)
message(STATUS "${Vulkan_LIBRARY} included at ${Vulkan_INCLUDE_DIR}")

# GLM
find_package(glm CONFIG REQUIRED)
set(GLM_LIBRARY glm::glm)
message(STATUS "glm included!")

# GLFW
find_package(glfw3 CONFIG REQUIRED)
set(GLFW_LIBRARY glfw)
message(STATUS "glfw3 included!")

# Define the include DIRs
set(LIB_INCL ${Vulkan_INCLUDE_DIR})
include_directories("${SRC_DIR}" "${INCL_DIR}" "${LIB_INCL}")

# Define the link libraries
link_directories("${LIB_DIR}")

# Put all libraries into a variable
set(LIBS ${Vulkan_LIBRARY} ${GLM_LIBRARY} ${GLFW_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${LIBS})
