cmake_minimum_required(VERSION 3.1) # target_sources

project(samples VERSION 0.0.0.1 LANGUAGES CXX)

add_executable(cconv)

target_include_directories(cconv
	PRIVATE
		../include)

target_sources(cconv
	PRIVATE
		cconv.cpp
		../src/hsv.cpp
		../src/hsl.cpp)

