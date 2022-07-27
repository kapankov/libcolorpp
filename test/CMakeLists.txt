cmake_minimum_required(VERSION 3.1) # target_sources

set(TEST_NAME run_test)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT>
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
    )
endif()

add_executable(${TEST_NAME} test.cpp ../src/hsv.cpp ../include/hsv.h ../src/hsl.cpp ../include/hsl.h ../src/rgb.cpp ../include/rgb.h)

target_include_directories(${TEST_NAME} PUBLIC ../include ../googletest/googletest/include)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
	message(STATUS "GNU CXX compiler")
	target_link_libraries(${TEST_NAME} gtest gtest_main pthread) # ${PROJECT_NAME}
else()
	message(STATUS "Not GNU CXX compiler")
	target_link_libraries(${TEST_NAME} gtest gtest_main) # ${PROJECT_NAME}
endif()


