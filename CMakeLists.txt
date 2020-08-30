cmake_minimum_required(VERSION 3.10)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wnon-virtual-dtor -Woverloaded-virtual -Wredundant-decls -Wno-sign-promo -Wno-sign-conversion ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 ${CMAKE_CXX_FLAGS}")

add_executable(Calculator Calculator.cpp Calculator.h main.cpp)