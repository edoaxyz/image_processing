cmake_minimum_required(VERSION 3.22)
project(image_processing)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tests)

include_directories(.)

set(SOURCE_FILES Image.cpp
        Image.h
        Kernel.cpp
        Kernel.h
        PGMManager.cpp
        PGMManager.h
        PPMManager.cpp
        PPMManager.h
        PXXManager.cpp
        PXXManager.h)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

add_library(core ${SOURCE_FILES})
add_executable(image_processing main.cpp)
target_link_libraries(image_processing core ${wxWidgets_LIBRARIES})
