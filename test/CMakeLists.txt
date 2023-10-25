include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tapes_tests
    tape_pool.cpp
    tape.cpp
    merge.cpp
    merge_tapes.cpp
    tape_view_write_iterator.cpp
    tape_view_read_iterator.cpp
    copy_elements_sorted.cpp
    merge_sort.cpp
    merge_sort_improved.cpp
    merge_sort_tests_utils.cpp
    improved_merge_sort_tests_utils.cpp
    merge_test_utils.cpp
)

target_link_libraries(tapes_tests LINK_PUBLIC gtest_main tape_simulation)

include(GoogleTest)
gtest_discover_tests(tapes_tests)
