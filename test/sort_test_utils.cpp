#include "sort_test_utils.hpp"

#include <sstream>
#include <random>

////////////////////////////////////////////////////////////////////////////////
std::vector<SortTestParam> generate_test_cases_of_sizes(
    std::uint32_t seed, std::initializer_list<std::size_t> sizes) {
  auto gen = std::mt19937(seed);

  std::vector<SortTestParam> ret;
  for (const auto size : sizes) {
    std::stringstream descrStream;
    descrStream << "generated_test_of_size_" << size;
    std::vector<std::int32_t> seq(size);
    for (auto& el: seq) {
      el = static_cast<std::int32_t>(gen());
    }
    ret.push_back(SortTestParam{descrStream.str(), std::move(seq)});
  }

  return ret;
}
