include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tapes_tests
    tape_view_fabric.cpp
    tape.cpp
)

target_link_libraries(tapes_tests LINK_PUBLIC gtest_main tape_simulation)

include(GoogleTest)
gtest_discover_tests(tapes_tests)
