# SimpleGameTemplate cmake build script.
cmake_minimum_required(VERSION 3.0)
project(Sysmel)

# Check the build type
if (CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Turn warnings and use C++ 2011.
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	# using Visual Studio C++
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
	if(UNIX)
		if(NOT ON_EMSCRIPTEN)
        	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
		endif()
    endif()
endif()

# Perform platform checks
include(${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
include(${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
include(${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
include(${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)

# Set output dir.
set(EXECUTABLE_OUTPUT_PATH "${Sysmel_BINARY_DIR}/dist")
set(LIBRARY_OUTPUT_PATH "${Sysmel_BINARY_DIR}/dist")

option(LIVE_CODING_SUPPORT True "Build with live coding support")

# Add the current directory.
include_directories(
.
"${PROJECT_SOURCE_DIR}/include"
"${PROJECT_SOURCE_DIR}/third-party/unittestpp"
"${PROJECT_BINARY_DIR}/include"
)

# Build the thirdparty dependencies
add_subdirectory(third-party)

# Build the libs
add_subdirectory(libs)

# Build the apps
add_subdirectory(apps)

# Build the tests
enable_testing()
add_subdirectory(tests)
