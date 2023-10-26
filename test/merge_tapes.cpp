#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <merge.hpp>
#include <tape_pool.hpp>
#include <tape_view_read_iterators.hpp>
#include <tape_view_write_iterators.hpp>

#include "merge_test_utils.hpp"
#include "common_utils.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers,
// cert-err58-cpp)

namespace {

////////////////////////////////////////////////////////////////////////////////
/// class MergeTapesTest for doing TEST_P
class MergeTapesTest : public testing::TestWithParam<MergeTestParam> {};

/**
 * Test of simple merging on tapes.
 * - Copies 2 input sequences from parameters to 2 tapes.
 * - Returns tapes heads to beginnings.
 * - Performs merge writing onto third tape.
 * - Copies data from third tape to vector.
 * - Checks result.
 */
TEST_P(MergeTapesTest, MergeSimple) {
  const auto& params = MergeTapesTest::GetParam();

  const auto tape0name = params.description + "_input_tape_0";
  const auto tape1name = params.description + "_input_tape_1";
  const auto outTapeName = params.description + "_output_tape";

  const auto itemsRemoved = remove_all(tape0name, tape1name, outTapeName);
  assert(itemsRemoved == 0 && "Some files were not removed.");

  const auto inSize0 = params.input0.size();
  const auto inSize1 = params.input1.size();
  const auto outSize = inSize0 + inSize1;

  {
    auto tp = TapePool();

    auto inTape0 = tp.createTape(tape0name, inSize0);
    auto inTape1 = tp.createTape(tape1name, inSize1);

    copy_n(params.input0.begin(), inSize0, RightWriteIterator(inTape0));
    copy_n(params.input1.begin(), inSize1, RightWriteIterator(inTape1));

    assert(inTape0.getPosition() + 1 == inSize0);
    assert(inTape1.getPosition() + 1 == inSize1);

    inTape0.moveLeftRepeated(inSize0 - 1);
    inTape1.moveLeftRepeated(inSize1 - 1);

    auto outTape = tp.createTape(outTapeName, outSize);

    auto in0 = RightReadIterator(inTape0);
    auto in1 = RightReadIterator(inTape1);
    auto out = RightWriteIterator(outTape);

    if (params.increasing) {
      merge_increasing(in0, inSize0, in1, inSize1, out);
    } else {
      merge_decreasing(in0, inSize0, in1, inSize1, out);
    }

    EXPECT_EQ(inTape0.getPosition(), inSize0 - 1);
    EXPECT_EQ(inTape1.getPosition(), inSize1 - 1);
    EXPECT_EQ(outTape.getPosition(), outSize - 1);

    auto resultVec = std::vector<std::int32_t>(outSize);
    copy_n(LeftReadIterator(outTape), outSize, resultVec.rbegin());

    std::vector<int> expected(params.input0.size() + params.input1.size());

    if (params.increasing) {
      std::merge(params.input0.begin(), params.input0.end(),
                 params.input1.begin(), params.input1.end(),
                 expected.begin());
    } else {
      std::merge(params.input0.rbegin(), params.input0.rend(),
                 params.input1.rbegin(), params.input1.rend(),
                 expected.rbegin());
    }
    EXPECT_TRUE(eq(expected, resultVec));
  }

  remove_all(tape0name, tape1name, outTapeName);
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

INSTANTIATE_TEST_SUITE_P(SimpleTapesMerges, MergeTapesTest,
                         testing::ValuesIn(simpleMergesInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.description;
                         });

const static auto generatedIncreasingMergeInputs =
    generate_merge_tapes_test_cases_of_sizes(
        42, true, {{2, 3}, {5, 5}, {6, 10}, {15, 16}, {31, 44}});

INSTANTIATE_TEST_SUITE_P(GeneratedIncreasingMerges, MergeTapesTest,
                         testing::ValuesIn(generatedIncreasingMergeInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.description;
                         });

const static auto generatedDecreasingMergeInputs =
    generate_merge_tapes_test_cases_of_sizes(
        42, false, {{2, 3}, {5, 5}, {6, 10}, {15, 16}, {31, 44}});

INSTANTIATE_TEST_SUITE_P(GeneratedDecreasingMerges, MergeTapesTest,
                         testing::ValuesIn(generatedDecreasingMergeInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.description;
                         });

}  // namespace

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers,
// cert-err58-cpp)
