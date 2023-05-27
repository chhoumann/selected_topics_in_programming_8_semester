include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)

FetchContent_MakeAvailable(googletest)

# Add test source files here
set(TEST_SOURCES main.cpp)

# Generate test executable
add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

# Link Google Test to the executable
target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main)

# Add the tests to be run by CTest
add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
