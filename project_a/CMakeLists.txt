cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
project(project_a)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Configure assets header file
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Vulkan
find_package(Vulkan REQUIRED)
message(STATUS "${Vulkan_LIBRARY} included at ${Vulkan_INCLUDE_DIR}")

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found ${GLFW3_LIBRARY} in ${GLFW3_INCLUDE_DIR}")

# Put all libraries into a variable
set(LIBS ${GLFW3_LIBRARY} ${Vulkan_LIBRARY})
set(INCLUDES ${GLFW3_INCLUDE_DIR} ${Vulkan_INCLUDE_DIR} ${GLM_INCLUDE_DIR})

# Define the include DIRs
include_directories("${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include")
include_directories(${INCLUDES})

# Define the link libraries
#link_directories(${LIBS})
target_link_libraries(${PROJECT_NAME} ${LIBS})

if (DEBUG_LEVEL)
    add_definitions(-DDEBUG_LEVEL=${DEBUG_LEVEL})
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
endif()


#[[# Symlink to res folder # http://qrikko.blogspot.com/2016/05/cmake-and-how-to-copy-resources-during.html
set(source "${CMAKE_SOURCE_DIR}/res")
set(dest "${CMAKE_CURRENT_BINARY_DIR}/res")
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${dest}
        DEPENDS ${dest}
        COMMENT "symbolic link resources folder from ${source} => ${dest}"
)]]

#[[
# Copy dlls
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/dlls"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()]]
