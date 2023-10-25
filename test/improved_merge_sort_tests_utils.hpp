#ifndef IMPROVED_MERGE_SORT_TESTS_UTILS_HPP
#define IMPROVED_MERGE_SORT_TESTS_UTILS_HPP

#include <cstdint>
#include <string>
#include <vector>

struct ImprovedMergeSortTestParam {
  std::string testDescription;
  std::vector<std::int32_t> values;
  std::size_t heapSizeLimit;
  bool increasing;
};

std::vector<ImprovedMergeSortTestParam>
generate_improved_merge_sort_test_cases_of_sizes(
    std::initializer_list<std::size_t> sizes,
    std::initializer_list<std::size_t> heapSizeLimits, bool increasing,
    std::uint32_t seed);

std::vector<ImprovedMergeSortTestParam>
generate_improved_merge_sort_test_cases_of_sizes_and_head_size_limits_ratios(
    std::initializer_list<std::size_t> sizes,
    std::initializer_list<std::size_t> heapSizeLimitsRatios, bool increasing,
    std::uint32_t seed);

#endif  // IMPROVED_MERGE_SORT_TESTS_UTILS_HPP
