cmake_minimum_required(VERSION 3.16)

# Set the project name
project(MyProject)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# To combat GoogleTest's use of deprecated copy constructor
add_compile_options(-Wno-deprecated-copy)

# Add sub-directories
add_subdirectory(src)

# Enable testing functionality
enable_testing()

# Add test sub-directory
add_subdirectory(tests)
