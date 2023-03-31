add_library(rk4 SHARED rk4one.cpp rk4two1.c rk4two2.c rk4two3.c rk4multi.cpp)
target_include_directories(rk4 PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

generate_export_header(rk4)

install(TARGETS rk4 DESTINATION ${LIBRARY_INSTALL_DIR})
install(FILES ${PROJECT_SOURCE_DIR}/include/rk4.h ${CMAKE_CURRENT_BINARY_DIR}/rk4_export.h
        DESTINATION include)

add_executable(test_rk4 test_rk4.c)
target_link_libraries(test_rk4 PUBLIC rk4 ${EXTRA_LIBS})
add_test(NAME test_rk4 COMMAND test_rk4)


add_executable(test_rk4_cpp test_rk4.cpp)
target_link_libraries(test_rk4_cpp rk4)


if(Qt5_FOUND)
    add_executable(test_qt5 test_qt5.cpp)
    target_link_libraries(test_qt5 PRIVATE Qt5::Charts Qt5::Widgets Qt5::Core)
    add_test(NAME test_qt5 COMMAND test_qt5)

    add_executable(plot test_plot.cpp plot.cpp)
    target_link_libraries(plot PRIVATE Qt5::Charts Qt5::Widgets Qt5::Core)
    add_test(NAME test_plot COMMAND test_plot)
endif()

add_test(NAME rk4one COMMAND python3 ${CMAKE_SOURCE_DIR}/py/rk4one.py $<TARGET_FILE:rk4>)
add_test(NAME rk4two1 COMMAND python3 ${CMAKE_SOURCE_DIR}/py/rk4two1.py $<TARGET_FILE:rk4>)
add_test(NAME rk4two2 COMMAND python3 ${CMAKE_SOURCE_DIR}/py/rk4two2.py $<TARGET_FILE:rk4>)
add_test(NAME rk4two3 COMMAND python3 ${CMAKE_SOURCE_DIR}/py/rk4two3.py $<TARGET_FILE:rk4>)
add_test(NAME rk4multi COMMAND python3 ${CMAKE_SOURCE_DIR}/py/rk4multi.py $<TARGET_FILE:rk4>)
