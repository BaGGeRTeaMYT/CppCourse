cmake_minimum_required(VERSION 3.24)

project(Seminar.21.09)

set(CMAKE_CXX_STANDARD 20)



include_directories(include/)

add_executable(Run main.cpp
               src/GameWisher.cpp
               src/Dictionary.cpp
               src/Wordly.cpp
               src/Console.cpp)
