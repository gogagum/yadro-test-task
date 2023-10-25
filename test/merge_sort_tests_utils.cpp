#include "merge_sort_tests_utils.hpp"

#include <random>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
std::vector<MergeSortTestParam> generate_merge_sort_test_cases_of_sizes(
    std::initializer_list<std::size_t> sizes, bool increasing,
    std::uint32_t seed) {
  auto gen = std::mt19937(seed);

  std::vector<MergeSortTestParam> ret;
  for (const auto size : sizes) {
    std::stringstream descrStream;
    descrStream << "generated_test_of_size_" << size;
    std::vector<std::int32_t> seq(size);
    for (auto& el : seq) {
      el = static_cast<std::int32_t>(gen());
    }
    ret.push_back(
        MergeSortTestParam{descrStream.str(), std::move(seq), increasing});
  }

  return ret;
}
