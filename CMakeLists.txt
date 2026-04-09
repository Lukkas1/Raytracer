cmake_minimum_required(VERSION 3.30)
project(Praktikum2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Praktikum2
        src/SimpleRayTracer.cpp
        src/vector.cpp
        src/rgbimage.cpp
        src/color.cpp
        src/CGUtilities.cpp
        src/main.cpp
)
