add_executable(meta_test meta_test.cpp)
target_link_libraries(meta_test PRIVATE doctest::doctest)

add_executable(json_output_test json_output_test.cpp)
target_link_libraries(json_output_test PRIVATE doctest::doctest)

add_executable(json_input_test json_input_test.cpp)
target_link_libraries(json_input_test PRIVATE doctest::doctest)

add_executable(move-exercise_test move-exercise_test.cpp)
target_link_libraries(move-exercise_test PRIVATE doctest::doctest)

add_test(NAME meta_test COMMAND meta_test)
add_test(NAME json_output_test COMMAND json_output_test)
add_test(NAME json_input_test COMMAND json_input_test)
add_test(NAME move-exercise_test COMMAND move-exercise_test)