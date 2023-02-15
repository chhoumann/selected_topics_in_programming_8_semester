cmake_minimum_required(VERSION 3.24)
project(polyline)

set(CMAKE_CXX_STANDARD 20)

add_executable(polyline main.cpp point.cpp point.h polyline.cpp polyline.h)