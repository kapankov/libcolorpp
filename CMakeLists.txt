cmake_minimum_required(VERSION 3.1) # target_sources

project(colorpp VERSION 0.0.0.1 LANGUAGES CXX)

#add_library(${PROJECT_NAME} STATIC)

#target_sources(${PROJECT_NAME} 
#	PRIVATE 
#		src/hsv.cpp
#		include/hsv.h
#		src/hsl.cpp
#		include/hsl.h)

# target_include_directories(${PROJECT_NAME} 
#	PUBLIC
#		include)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
SET(GCC_DEBUG_FLAGS "-g -Wall")

# Add Code Coverage Build Type
message("* Adding build types...")
SET(CMAKE_CXX_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )
endif()


# gtest

# We want to build only GoogleTest
option(BUILD_GMOCK OFF)

add_subdirectory(googletest)

add_subdirectory(test)

add_subdirectory(samples)

