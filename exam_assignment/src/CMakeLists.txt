# Add source files here
set(SOURCES main.cpp symbol_table.cpp stochastic_simulator.cpp)

# Generate executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link graphviz
target_link_libraries(MyProject cgraph gvc)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)