cmake_minimum_required(VERSION 3.22)
project(image_processing)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(image_processing
        main.cpp
        Image.cpp
        Image.h
        )

add_subdirectory(tests)