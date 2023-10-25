#ifndef TEST_SORT_TESTS_UTILS_HPP
#define TEST_SORT_TESTS_UTILS_HPP

#include <cstdint>
#include <string>
#include <vector>

struct MergeSortTestParam {
  std::string testDescription;
  std::vector<std::int32_t> values;
  bool increasing;
};

std::vector<MergeSortTestParam> generate_merge_sort_test_cases_of_sizes(
    std::initializer_list<std::size_t> sizes, bool increasing,
    std::uint32_t seed);

#endif  // TEST_GENERATE_SORT_TESTS_HPP
