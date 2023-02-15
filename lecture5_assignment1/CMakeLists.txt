cmake_minimum_required(VERSION 3.24)
project(NumberSeries)

set(CMAKE_CXX_STANDARD 20)

add_executable(NumberSeries main.cpp number_series.cpp number_series.h series_wrapper.cpp series_wrapper.h)