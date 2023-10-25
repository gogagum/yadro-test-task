#include <gtest/gtest.h>

#include <copy_n.hpp>
#include <filesystem>
#include <merge_sort.hpp>
#include <vector>

#include "common_utils.hpp"
#include "merge_sort_tests_utils.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-avoid-magic-numbers, cert-err58-cpp)

namespace {

class MergeSortTest : public testing::TestWithParam<MergeSortTestParam> {};

TEST_P(MergeSortTest, MergeSortCompareWithStdSort) {
  const auto& params = MergeSortTest::GetParam();
  const auto inFilename = params.testDescription + "_in_file";
  const auto outFilename = params.testDescription + "_out_file";

  remove_all(inFilename, outFilename, "tmp");

  {
    auto tapePool = TapePool();
    auto inTape = tapePool.createTape(inFilename, params.values.size());
    copy_n(params.values.begin(), params.values.size(),
           RightWriteIterator(inTape));

    inTape.moveLeftRepeated(inTape.getPosition());

    MergeSort(tapePool, inFilename, "tmp", params.increasing)
        .perform(outFilename);

    auto outTape = tapePool.getOpenedTape(outFilename);
    outTape.moveLeftRepeated(outTape.getPosition());

    auto result = std::vector<std::int32_t>{};

    copy_n(RightReadIterator(outTape), params.values.size(),
           std::back_inserter(result));

    auto expected = std::vector<std::int32_t>(params.values);
    std::sort(expected.begin(), expected.end(),
              [&params](auto v1, auto v2) -> bool {
                return params.increasing ? (v1 < v2) : (v1 > v2);
              });

    EXPECT_TRUE(eq(expected, result));
  }

  remove_all(inFilename, outFilename);
}

static auto simpleMergeSortInputs = std::vector<MergeSortTestParam>{
    {"sort_only_element_increasing", {42}, true},
    {"sort_only_element_decreasing", {42}, false},
    {"sort_two_sorted_elements_increasing", {42, 57}, true},
    {"sort_two_unsorted_elements_increasing", {57, 42}, true},
    {"sort_two_equal_elements_increasing", {42, 42}, true},
    {"sort_three_elements_increasing", {1, 3, 2}, true},
    {"sort_three_equal_increasing", {1, 1, 1}, true},
    {"sort_three_sorted_increasing", {1, 2, 3}, true},
    {"sort_four_increasing", {2, 3, 1, 4}, true},
    {"sort_four_sorted_increasing", {1, 2, 3, 4}, true},
    {"sort_four_decreasing_increasing", {4, 3, 2, 1}, true},
    {"sort_four_decreasing", {2, 3, 1, 4}, false},
    {"sort_four_sorted_decreasing", {1, 2, 3, 4}, false},
    {"sort_four_decreasing_decreasing", {4, 3, 2, 1}, false},
    {"sort_five_increasing", {5, 3, 2, 1, 4}, true},
    {"sort_five2_increasing", {5, 1, 2, 4, 3}, true},
    {"sort_five_sorted_increasing", {1, 2, 3, 4, 5}, true},
    {"sort_five_decreasing_increasing", {5, 4, 3, 2, 1}, true},
    {"sort_10_increasing", {9, 4, 5, 3, 1, 2, 7, 8, 6, 10}, true}};

INSTANTIATE_TEST_SUITE_P(SimpleTapesMergeSorts, MergeSortTest,
                         testing::ValuesIn(simpleMergeSortInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.testDescription;
                         });

static auto twoPowSizedIncreasingTestCases = generate_merge_sort_test_cases_of_sizes(
    {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048}, true, 42);

INSTANTIATE_TEST_SUITE_P(TwoPowSizedMergeSorts, MergeSortTest,
                         testing::ValuesIn(twoPowSizedIncreasingTestCases),
                         [](const auto& paramInfo) {
                           return paramInfo.param.testDescription;
                         });

static auto twoPowSizedDecreasingTestCases = generate_merge_sort_test_cases_of_sizes(
    {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048}, false, 42);

INSTANTIATE_TEST_SUITE_P(TwoPowSizedDecreasingMergeSorts, MergeSortTest,
                         testing::ValuesIn(twoPowSizedDecreasingTestCases),
                         [](const auto& paramInfo) {
                           return paramInfo.param.testDescription;
                         });

static auto threePowIncreasingSizedTestCases =
    generate_merge_sort_test_cases_of_sizes({1, 3, 9, 27, 81, 243, 729}, true, 42);

INSTANTIATE_TEST_SUITE_P(ThreePowSizedIncreasingMergeSorts, MergeSortTest,
                         testing::ValuesIn(threePowIncreasingSizedTestCases),
                         [](const auto& paramInfo) {
                           return paramInfo.param.testDescription;
                         });

static auto threePowDecreasingSizedTestCases =
    generate_merge_sort_test_cases_of_sizes({1, 3, 9, 27, 81, 243, 729}, false, 42);

INSTANTIATE_TEST_SUITE_P(ThreePowSizedDecreasingMergeSorts, MergeSortTest,
                         testing::ValuesIn(threePowDecreasingSizedTestCases),
                         [](const auto& paramInfo) {
                           return paramInfo.param.testDescription;
                         });

}  // namespace

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-avoid-magic-numbers, cert-err58-cpp)
