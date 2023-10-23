#ifndef TEST_SORT_TESTS_UTILS_HPP
#define TEST_SORT_TESTS_UTILS_HPP

#include <cstdint>
#include <string>
#include <vector>

struct SortTestParam {
  std::string testDescription;
  std::vector<std::int32_t> values;
};

std::vector<SortTestParam> generate_test_cases_of_sizes(
    std::uint32_t seed, std::initializer_list<std::size_t> sizes);

#endif  // TEST_GENERATE_SORT_TESTS_HPP
