#include <gtest/gtest.h>

#include <copy_n.hpp>
#include <filesystem>
#include <merge_sort_improved.hpp>
#include <vector>

#include "common_utils.hpp"
#include "improved_merge_sort_tests_utils.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-avoid-magic-numbers, cert-err58-cpp)

namespace {

class ImprovedMergeSortTest
    : public testing::TestWithParam<ImprovedMergeSortTestParam> {};

TEST_P(ImprovedMergeSortTest, MergeSortCompareWithStdSort) {
  const auto& params = ImprovedMergeSortTest::GetParam();
  const auto inFilename = params.testDescription + "_in_file";
  const auto outFilename = params.testDescription + "_out_file";

  remove_all(inFilename, outFilename, "tmp");

  {
    auto tapePool = TapePool();
    auto inTape = tapePool.createTape(inFilename, params.values.size());
    copy_n(params.values.begin(), params.values.size(),
           RightWriteIterator(inTape));

    inTape.moveLeftRepeated(inTape.getPosition());

    MergeSortImproved(tapePool, inFilename, "tmp", params.increasing,
                      params.heapSizeLimit)
        .perform(outFilename);

    auto outTape = tapePool.getOpenedTape(outFilename);
    outTape.moveLeftRepeated(outTape.getPosition());

    auto result = std::vector<std::int32_t>{};

    copy_n(RightReadIterator(outTape), params.values.size(),
           std::back_inserter(result));

    auto expected = std::vector<std::int32_t>(params.values);
    std::sort(expected.begin(), expected.end(), [&params](auto v0, auto v1) {
      return params.increasing ? (v0 < v1) : (v0 > v1);
    });

    EXPECT_TRUE(eq(expected, result));
  }

  remove_all(inFilename, outFilename);
}

const static auto simpleMergeSortInputs =
    std::vector<ImprovedMergeSortTestParam>{
        {"sort_only_element", {42}, 1, true},
        {"sort_two_sorted_elements_with_heap", {42, 57}, 5, true},
        {"sort_two_unsorted_elements", {57, 42}, 1, true},
        {"sort_two_equal_elements", {42, 42}, 1, true},
        {"sort_three_elements_with_heap_2", {1, 3, 2}, 2, true},
        {"sort_three_equal", {1, 1, 1}, 2, true},
        {"sort_three_sorted", {1, 2, 3}, 2, true},
        {"sort_four", {2, 3, 1, 4}, 3, true},
        {"sort_four_sorted", {1, 2, 3, 4}, 2, true},
        {"sort_four_decreasing", {4, 3, 2, 1}, 2, true},
        {"sort_five", {5, 3, 2, 1, 4}, 3, true},
        {"sort_five2", {5, 1, 2, 4, 3}, 3, true},
        {"sort_five_sorted", {1, 2, 3, 4, 5}, 3, true},
        {"sort_five_decreasing", {5, 4, 3, 2, 1}, 3, true},
        {"sort_10", {9, 4, 5, 3, 1, 2, 7, 8, 6, 10}, 3, true}};

INSTANTIATE_TEST_SUITE_P(SimpleTapesMergeSorts, ImprovedMergeSortTest,
                         testing::ValuesIn(simpleMergeSortInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.testDescription;
                         });

const static auto twoPowSizedTestCases =
    generate_improved_merge_sort_test_cases_of_sizes(
        {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048}, {1, 3, 13, 103},
        true, 42);

INSTANTIATE_TEST_SUITE_P(TwoPowSizedMergeSorts, ImprovedMergeSortTest,
                         testing::ValuesIn(twoPowSizedTestCases),
                         [](const auto& paramInfo) {
                           return paramInfo.param.testDescription;
                         });

const static auto threePowSizedTestCases =
    generate_improved_merge_sort_test_cases_of_sizes(
        {1, 3, 9, 27, 81, 243, 729}, {1, 3, 13, 103}, true, 42);

INSTANTIATE_TEST_SUITE_P(ThreePowSized, ImprovedMergeSortTest,
                         testing::ValuesIn(threePowSizedTestCases),
                         [](const auto& paramInfo) {
                           return paramInfo.param.testDescription;
                         });

const static auto twoPowSizedIncreasingTestCasesWithRatios =
    generate_improved_merge_sort_test_cases_of_sizes_and_head_size_limits_ratios(
        {16, 32, 64, 128, 256, 512, 1024, 2048}, {1, 2, 4, 8}, true, 57);

INSTANTIATE_TEST_SUITE_P(
    IncreasingTwoPowSizedWithRatios, ImprovedMergeSortTest,
    testing::ValuesIn(twoPowSizedIncreasingTestCasesWithRatios),
    [](const auto& paramInfo) {
      return paramInfo.param.testDescription;
    });


const static auto twoPowSizedDecreasingTestCasesWithRatios =
    generate_improved_merge_sort_test_cases_of_sizes_and_head_size_limits_ratios(
        {16, 32, 64, 128, 256, 512, 1024, 2048}, {1, 2, 4, 8}, false, 57);

INSTANTIATE_TEST_SUITE_P(
    DecreasingTwoPowSizedWithRatios, ImprovedMergeSortTest,
    testing::ValuesIn(twoPowSizedDecreasingTestCasesWithRatios),
    [](const auto& paramInfo) {
      return paramInfo.param.testDescription;
    });

}  // namespace

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-avoid-magic-numbers, cert-err58-cpp)
