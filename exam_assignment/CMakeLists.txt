cmake_minimum_required(VERSION 3.26)

# Set the project name
project(StochasticSimulator)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library symbol visibility: hide by default, unless asked to export
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
# For library export header generation:
include(GenerateExportHeader)

include(sanitizers.cmake)

# Qt5
find_package(Qt5 COMPONENTS Widgets Charts QUIET)
if (Qt5_FOUND)
    message(STATUS "Qt5 found, fantastic!")
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
else(Qt5_FOUND)
    message(WARNING "Qt5 NOT found, test_qt5 will be disabled. Please install qt5charts development package.")
endif(Qt5_FOUND)

# To combat GoogleTest's use of deprecated copy constructor
add_compile_options(-Wno-deprecated-copy)

# Add sub-directories
add_subdirectory(src)

# Enable testing functionality
enable_testing()

# Add test sub-directory
add_subdirectory(tests)