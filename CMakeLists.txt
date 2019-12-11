cmake_minimum_required(VERSION 3.15)
project(Emu VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW without examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW without tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW without docs")
set(GLFW_INSTALL OFF CACHE BOOL "Build GLFW without install")

add_subdirectory(Source/Ras2D)

include_directories(Source)
include_directories(Source/Ras2D/src)

file (GLOB SOURCES Source/*.cpp src/*/*.cpp)

if (APPLE)
    add_executable(Emu MACOSX_BUNDLE ${SOURCES})
elseif(WIN32)
    add_executable(Emu WIN32 ${SOURCES})
else()
    add_executable(Emu ${SOURCES})
endif()

target_link_libraries(Emu Ras2D)
target_link_libraries(Emu glfw)
