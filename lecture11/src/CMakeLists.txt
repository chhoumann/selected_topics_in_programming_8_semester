add_executable(digits_test digits_test.cpp)
target_compile_definitions(digits_test PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
target_link_libraries(digits_test PRIVATE doctest::doctest)
add_test(NAME digits_test COMMAND digits_test)

add_executable(digits_bm digits_bm.cpp)
target_link_libraries(digits_bm PRIVATE benchmark::benchmark)

add_library(stringsort OBJECT stringsort.cpp)

add_executable(stringsort_test stringsort_test.cpp)
target_compile_definitions(stringsort_test PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
target_link_libraries(stringsort_test PRIVATE stringsort doctest::doctest)
add_test(NAME stringsort_test COMMAND stringsort_test)

add_executable(stringsort0_bm stringsort0_bm.cpp)
target_link_libraries(stringsort0_bm PRIVATE stringsort benchmark::benchmark)

add_executable(stringsort1_bm stringsort1_bm.cpp)
target_link_libraries(stringsort1_bm PRIVATE stringsort benchmark::benchmark)

add_executable(stringsort2_bm stringsort2_bm.cpp)
target_link_libraries(stringsort2_bm PRIVATE stringsort benchmark::benchmark)

if(OPENMP_FOUND AND HAVE_GNU_PARALLEL)
    add_executable(stringsort3_bm stringsort3_bm.cpp)
    target_link_libraries(stringsort3_bm PRIVATE stringsort OpenMP::OpenMP_CXX benchmark::benchmark)
endif()

if(TBB_FOUND)
    add_executable(stringsort4_bm stringsort4_bm.cpp)
    target_link_libraries(stringsort4_bm PRIVATE stringsort TBB::tbb benchmark::benchmark)
endif()

add_executable(mtqueue mtqueue.cpp)
target_link_libraries(mtqueue PRIVATE Threads::Threads)

add_executable(futures futures.cpp)
target_link_libraries(futures PRIVATE Threads::Threads)

option(TSAN "Thread Sanitizer" ON)