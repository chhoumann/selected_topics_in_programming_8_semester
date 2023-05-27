# Add source files here
set(SOURCES main.cpp)

# Generate executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link graphviz
target_link_libraries(MyProject cgraph gvc)