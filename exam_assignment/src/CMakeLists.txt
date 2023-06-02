# Add source files here
set(SOURCES main.cpp types.cpp stochastic_simulator.cpp monitor/monitors.cpp plot/plot.cpp exercises/make_graphs.h exercises/benchmark.h graph_generator.h)

# Generate executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link graphviz
target_link_libraries(MyProject cgraph gvc Qt5::Charts Qt5::Widgets Qt5::Core)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)