#include "merge_test_utils.hpp"

#include <random>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
std::vector<MergeTestParam> generate_merge_tapes_test_cases_of_sizes(
    std::uint32_t seed, bool increasing,
    std::initializer_list<std::pair<std::size_t, std::size_t>> sizes) {
  auto gen = std::mt19937(seed);
  constexpr auto maxStep = std::int32_t{100};

  std::vector<MergeTestParam> ret;
  for (const auto size : sizes) {
    std::stringstream descrStream;
    descrStream << "generated_test_of_merged_sizes_" << size.first << "_and_"
                << size.second;
    std::vector<std::int32_t> seq0(size.first);
    std::vector<std::int32_t> seq1(size.second);

    {
      auto curr0 = static_cast<std::int32_t>(gen());
      for (auto& el : seq0) {
        if (increasing) {
          curr0 += static_cast<std::int32_t>(gen()) % maxStep;
        } else {
          curr0 -= static_cast<std::int32_t>(gen()) % maxStep;
        }
        el = curr0;
      }
    }

    {
      auto curr1 = static_cast<std::int32_t>(gen());
      for (auto& el : seq1) {
        if (increasing) {
          curr1 += static_cast<std::int32_t>(gen()) % maxStep;
        } else {
          curr1 -= static_cast<std::int32_t>(gen()) % maxStep;
        }
        el = curr1;
      }
    }

    ret.push_back(MergeTestParam{descrStream.str(), std::move(seq0),
                                 std::move(seq1), increasing});
  }

  return ret;
}