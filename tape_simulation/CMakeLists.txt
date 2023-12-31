cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(tape_simulation STATIC)

target_sources(tape_simulation
    PRIVATE
        src/tape_pool.cpp
        src/tape_view.cpp
        src/tape.cpp
        src/tape_view_write_iterators.cpp
        src/tape_view_read_iterators.cpp
        src/merge_sort.cpp
        src/improved_merge_sort.cpp
        src/copy_n.cpp
        src/merge.cpp
        src/merge_sort_impl.cpp
        src/merge_sort_impl_checks.cpp
        src/merge_sort_additional_tapes_manager.cpp
        src/merge_sort_arithmetics_base.cpp
        src/copy_elements_sorted.cpp
)

target_include_directories(tape_simulation
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(public_headers
    include/heap_part_sort.hpp
    include/tape_pool.hpp
    include/tape_view.hpp
    include/tape.hpp
    include/tape_view_write_iterators.hpp
    include/move_top_elements_sorted.hpp
    include/copy_top_elements_sorted.hpp
    include/merge.hpp
    include/merge_sort.hpp
    include/merge_sort_improved.hpp
    include/copy_n.hpp
)

set_target_properties(tape_simulation PROPERTIES PUBLIC_HEADER "${public_headers}")
set_target_properties(tape_simulation PROPERTIES LINKER_LANGUAGE CXX)

