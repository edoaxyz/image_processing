cmake_minimum_required(VERSION 3.22)
project(image_processing)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(tests)

include_directories(.)

add_executable(image_processing
        main.cpp
        Image.cpp
        Image.h
        Kernel.cpp
        Kernel.h
        PGMManager.cpp
        PGMManager.h
        PPMManager.cpp
        PPMManager.h
        PXXManager.cpp
        PXXManager.h
        )
