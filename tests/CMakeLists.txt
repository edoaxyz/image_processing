cmake_minimum_required(VERSION 3.22)
project(tests)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCE_FILES runAllTests.cpp ImageTest.cpp KernelTest.cpp ImageProcessingTest.cpp PGMTest.cpp PPMTest.cpp)
add_executable(tests_run ${SOURCE_FILES})
target_link_libraries(tests_run gtest gtest_main core)