#include "improved_merge_sort_tests_utils.hpp"

#include <random>
#include <sstream>

namespace {

std::string generate_name(std::size_t size, std::size_t heapSizeLimit) {
  std::stringstream descrStream;
  descrStream << "generated_test_of_size_" << size << "_with_heap_size_limit_"
              << heapSizeLimit;
  return descrStream.str();
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
std::vector<ImprovedMergeSortTestParam>
generate_improved_merge_sort_test_cases_of_sizes(
    std::initializer_list<std::size_t> sizes,
    std::initializer_list<std::size_t> heapSizeLimits, bool increasing,
    std::uint32_t seed) {
  auto gen = std::mt19937(seed);
  std::vector<ImprovedMergeSortTestParam> ret;
  for (const auto size : sizes) {
    std::vector<std::int32_t> seq(size);
    for (auto& el : seq) {
      el = static_cast<std::int32_t>(gen());
    }
    for (auto heapSizeLimit : heapSizeLimits) {
      ret.push_back(
          {generate_name(size, heapSizeLimit), seq, heapSizeLimit, increasing});
    }
  }
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<ImprovedMergeSortTestParam>
generate_improved_merge_sort_test_cases_of_sizes_and_head_size_limits_ratios(
    std::initializer_list<std::size_t> sizes,
    std::initializer_list<std::size_t> heapSizeLimitsRatios, bool increasing,
    std::uint32_t seed) {
  auto gen = std::mt19937(seed);
  std::vector<ImprovedMergeSortTestParam> ret;
  for (const auto size : sizes) {
    std::vector<std::int32_t> seq(size);
    for (auto& el : seq) {
      el = static_cast<std::int32_t>(gen());
    }
    for (auto heapSizeLimitRatio : heapSizeLimitsRatios) {
      const auto heapSizeLimit =
          std::max<std::size_t>(1, size / heapSizeLimitRatio);
      ret.push_back(
          {generate_name(size, heapSizeLimit), seq, heapSizeLimit, increasing});
    }
  }

  return ret;
}
