#ifndef TEST_MERGE_TEST_UTILS_HPP
#define TEST_MERGE_TEST_UTILS_HPP

#include <cstdint>
#include <string>
#include <vector>

struct MergeTestParam {
  std::string description;
  std::vector<std::int32_t> input0;
  std::vector<std::int32_t> input1;
  bool increasing;
};

std::vector<MergeTestParam> generate_merge_tapes_test_cases_of_sizes(
    std::uint32_t seed,
    bool increasing,
    std::initializer_list<std::pair<std::size_t, std::size_t>> sizes);

#endif  // TEST_MERGE_TEST_UTILS_HPP