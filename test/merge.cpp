#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <merge.hpp>
#include <random>

#include "common_utils.hpp"
#include "merge_test_utils.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)

namespace {

////////////////////////////////////////////////////////////////////////////////
/// class MergeTest for doing TEST_P
class MergeTest : public testing::TestWithParam<MergeTestParam> {};

TEST_P(MergeTest, MergeSimple) {
  const auto& params = MergeTest::GetParam();

  auto result = std::vector<int>{};
  auto expected = std::vector<int>(params.input0.size() + params.input1.size());

  if (params.increasing) {
    merge_increasing(params.input0.begin(), params.input0.size(),
                     params.input1.begin(), params.input1.size(),
                     std::back_inserter(result));
    std::merge(params.input0.begin(), params.input0.end(),
               params.input1.begin(), params.input1.end(), expected.begin());

  } else {
    merge_decreasing(params.input0.begin(), params.input0.size(),
                     params.input1.begin(), params.input1.size(),
                     std::back_inserter(result));
    std::merge(params.input0.rbegin(), params.input0.rend(),
               params.input1.rbegin(), params.input1.rend(), expected.rbegin());
  }

  EXPECT_TRUE(eq(result, expected));
}

static auto simpleMergesInputs = std::vector<MergeTestParam>{
    {"merge_one_elements_tapes_increasing", {42}, {57}, true},
    {"merge_one_elements_tapes_decreasing", {42}, {57}, false},
    {"merge_one_element_and_multiple_increasing", {33}, {12, 24, 37}, true},
    {"merge_one_element_and_multiple_decreasing", {33}, {37, 24, 12}, false},
    {"merge_intersecting_increasing", {1, 3, 4}, {2, 5, 6}, true},
    {"merge_intersecting_decreasing", {4, 3, 1}, {6, 5, 2}, false},
    {"merge_non_intersecting_increasing", {1, 2, 3}, {4, 5, 6}, true},
    {"merge_non_intersecting_decreasing", {3, 2, 1}, {6, 5, 4}, false},
    {"merge_non_intersecting_increasing_swapped", {4, 5, 6}, {1, 2, 3}, true},
    {"merge_non_intersecting_decreasing_swapped", {6, 5, 4}, {3, 2, 1}, false}};

INSTANTIATE_TEST_SUITE_P(SimpleTapesMerges, MergeTest,
                         testing::ValuesIn(simpleMergesInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.description;
                         });

const static auto generatedIncreasingMergeInputs =
    generate_merge_tapes_test_cases_of_sizes(
        42, true, {{2, 3}, {5, 5}, {6, 10}, {15, 16}, {31, 44}});

INSTANTIATE_TEST_SUITE_P(GeneratedIncreasingMerges, MergeTest,
                         testing::ValuesIn(generatedIncreasingMergeInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.description;
                         });

const static auto generatedDecreasingMergeInputs =
    generate_merge_tapes_test_cases_of_sizes(
        42, false, {{2, 3}, {5, 5}, {6, 10}, {15, 16}, {31, 44}});

INSTANTIATE_TEST_SUITE_P(GeneratedDecreasingMerges, MergeTest,
                         testing::ValuesIn(generatedDecreasingMergeInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.description;
                         });

}  // namespace

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeTwoEmptyIncreasing) {
  const auto src = std::array{1, 2, 3};
  auto target = std::vector<std::uint32_t>{};

  const auto perform = [&]() {
    merge_increasing(src.begin(), 0, src.begin(), 0,
                     std::back_inserter(target));
  };

  EXPECT_THROW(perform(), std::logic_error);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeTwoEmptyDecreasing) {
  const auto src = std::array{1, 2, 3};
  auto target = std::vector<std::uint32_t>{};

  const auto perform = [&]() {
    merge_decreasing(src.begin(), 0, src.begin(), 0,
                     std::back_inserter(target));
  };

  EXPECT_THROW(perform(), std::logic_error);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeIncreasingWhileOneIsEmpty) {
  const auto src = std::array{1, 2, 3};
  auto target1 = std::vector<std::uint32_t>{};
  auto target2 = std::vector<std::uint32_t>{};

  const auto perform0 = [&]() {
    merge_increasing(src.begin(), 3, src.begin(), 0,
                     std::back_inserter(target1));
  };

  EXPECT_THROW(perform0(), std::logic_error);

  const auto perform1 = [&]() {
    merge_increasing(src.begin(), 0, src.begin(), 3,
                     std::back_inserter(target2));
  };

  EXPECT_THROW(perform1(), std::logic_error);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeDecreasingWhileOneIsEmpty) {
  const auto src = std::array{3, 2, 1};
  auto target1 = std::vector<std::uint32_t>{};
  auto target2 = std::vector<std::uint32_t>{};

  const auto perform0 = [&]() {
    merge_decreasing(src.begin(), 3, src.begin(), 0,
                     std::back_inserter(target1));
  };

  EXPECT_THROW(perform0(), std::logic_error);

  const auto perform1 = [&]() {
    merge_decreasing(src.begin(), 0, src.begin(), 3,
                     std::back_inserter(target2));
  };

  EXPECT_THROW(perform1(), std::logic_error);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeTwoEqualIncreasing) {
  const auto src = std::array{1, 2, 3};
  auto target = std::vector<std::uint32_t>{};

  const auto expected = std::array{1, 1, 2, 2, 3, 3};

  merge_increasing(src.begin(), 3, src.begin(), 3, std::back_inserter(target));
  EXPECT_EQ(target.size(), 6);
  EXPECT_TRUE(std::equal(target.begin(), target.end(), expected.begin()));
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeTwoEqualDecreasing) {
  const auto src = std::array{3, 2, 1};
  auto target = std::vector<std::uint32_t>{};

  const auto expected = std::array{3, 3, 2, 2, 1, 1};

  merge_decreasing(src.begin(), 3, src.begin(), 3, std::back_inserter(target));
  EXPECT_EQ(target.size(), 6);
  EXPECT_TRUE(std::equal(target.begin(), target.end(), expected.begin()));
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeTwoIncreasing) {
  const auto src1 = std::array{1, 3, 4};
  const auto src2 = std::array{2, 5, 6};
  auto target = std::vector<std::uint32_t>{};

  const auto expected = std::array{1, 2, 3, 4, 5, 6};

  merge_increasing(src1.begin(), 3, src2.begin(), 3,
                   std::back_inserter(target));
  EXPECT_EQ(target.size(), 6);
  EXPECT_TRUE(std::equal(target.begin(), target.end(), expected.begin()));
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeTwoDecreasing) {
  const auto src1 = std::array{4, 3, 1};
  const auto src2 = std::array{6, 5, 2};
  auto target = std::vector<std::uint32_t>{};

  const auto expected = std::array{6, 5, 4, 3, 2, 1};

  merge_decreasing(src1.begin(), 3, src2.begin(), 3,
                   std::back_inserter(target));
  EXPECT_EQ(target.size(), 6);
  EXPECT_TRUE(std::equal(target.begin(), target.end(), expected.begin()));
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, FuzzIncreasing) {
  auto gen = std::mt19937(42);

  for (std::size_t i = 0; i < 100; ++i) {
    auto cnt1 = std::size_t{gen() % 100};
    auto cnt2 = std::size_t{gen() % 100};

    auto seq1 = std::vector<std::int32_t>{};
    seq1.reserve(cnt1);

    {
      auto currValue = static_cast<std::int32_t>(gen() % 10);
      for (std::size_t j = 0; j < cnt1; ++j, currValue += (gen() % 10)) {
        seq1.push_back(currValue);
      }
    }

    auto seq2 = std::vector<std::int32_t>{};
    seq2.reserve(cnt2);

    {
      auto currValue = static_cast<std::int32_t>(gen() % 10);
      for (std::size_t j = 0; j < cnt2; ++j, currValue += (gen() % 10)) {
        seq2.push_back(currValue);
      }
    }

    auto expected = std::vector<std::int32_t>{};
    expected.reserve(cnt1 + cnt2);

    std::merge(seq1.begin(), seq1.end(), seq2.begin(), seq2.end(),
               std::back_inserter(expected));

    auto result = std::vector<std::int32_t>{};
    result.reserve(cnt1 + cnt2);

    if (seq1.size() != 0 && seq2.size() != 0) {
      merge_increasing(seq1.begin(), seq1.size(), seq2.begin(), seq2.size(),
                       std::back_inserter(result));

      EXPECT_TRUE(eq(expected, result));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, FuzzDecreasing) {
  auto gen = std::mt19937(42);

  for (std::size_t i = 0; i < 100; ++i) {
    auto cnt1 = std::size_t{gen() % 100 + 1};
    auto cnt2 = std::size_t{gen() % 100 + 1};

    auto seq1 = std::vector<std::int32_t>{};
    seq1.reserve(cnt1);

    {
      auto currValue = static_cast<std::int32_t>(gen() % 10);
      for (std::size_t j = 0; j < cnt1; ++j, currValue -= (gen() % 10)) {
        seq1.push_back(currValue);
      }
    }

    auto seq2 = std::vector<std::int32_t>{};
    seq2.reserve(cnt2);

    {
      auto currValue = static_cast<std::int32_t>(gen() % 10);
      for (std::size_t j = 0; j < cnt2; ++j, currValue -= (gen() % 10)) {
        seq2.push_back(currValue);
      }
    }

    auto expected = std::vector<std::int32_t>(cnt1 + cnt2);

    std::merge(seq1.rbegin(), seq1.rend(), seq2.rbegin(), seq2.rend(),
               std::rbegin(expected));

    auto result = std::vector<std::int32_t>{};
    merge_decreasing(seq1.begin(), seq1.size(), seq2.begin(), seq2.size(),
                     std::back_inserter(result));

    EXPECT_TRUE(eq(expected, result));
  }
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)
