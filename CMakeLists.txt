cmake_minimum_required(VERSION 3.2)
project(3DViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(TDVIEWER_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(TDVIEWER_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_definitions(TDVIEWER_RELWITHDEBINFO)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_definitions(TDVIEWER_MINSIZEREL)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin-int/${CMAKE_BUILD_TYPE})

set(GLFW_DIR "${CMAKE_SOURCE_DIR}/dependencies/glfw")
add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_DIR}/include)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")

include_directories(dependencies/glm/glm)

include_directories(dependencies/glad/src)
include_directories(dependencies/glad/include)

add_library(glad dependencies/glad/src/glad.c)

include_directories(dependencies/imgui)

file(GLOB_RECURSE SOURCES "src/*.hpp" "src/*.cpp" "src/*.h")
add_executable(3DViewer ${SOURCES})
set(GLFW_BUILD_WAYLAND ON)
if(WIN32) # Pokud je operační systém Windows
    set(CMAKE_C_COMPILER "cl")
    set(CMAKE_CXX_COMPILER "cl")

    target_link_libraries(3DViewer glad glfw)
elseif(UNIX AND NOT APPLE) # Pokud je operační systém Linux
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsized-deallocation")

    if(NOT DEFINED CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE Release)
	endif()
	target_link_libraries(3DViewer PUBLIC glad glfw pthread)
endif()
