cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

project(tape_simulation)

add_subdirectory(tape_simulation)

add_subdirectory(test)

include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

add_executable(write_from_console src/write_from_console.cpp)
target_link_libraries(write_from_console PRIVATE tape_simulation argparse)

add_executable(read_to_console src/read_to_console.cpp)
target_link_libraries(read_to_console PRIVATE tape_simulation argparse)

add_executable(sort_simple src/sort_simple.cpp)
target_link_libraries(sort_simple PRIVATE tape_simulation argparse)

add_executable(sort_improved src/sort_improved.cpp)
target_link_libraries(sort_improved PRIVATE tape_simulation argparse)
