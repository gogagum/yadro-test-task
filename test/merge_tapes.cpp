#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <merge.hpp>
#include <tape_pool.hpp>
#include <tape_view_read_iterators.hpp>
#include <tape_view_write_iterators.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers,
// cert-err58-cpp)

namespace {

////////////////////////////////////////////////////////////////////////////////
/// class MergeTapesTestParam - simple tapes merge parameter type
struct MergeTapesTestParam {
  std::string description;
  std::vector<std::int32_t> input0;
  std::vector<std::int32_t> input1;
  bool increasing;
};

////////////////////////////////////////////////////////////////////////////////
/// class MergeTapesTest for doing TEST_P
class MergeTapesTest : public testing::TestWithParam<MergeTapesTestParam> {};

/**
 * Test of simple merging on tapes.
 * - Copies 2 input sequences from parameters to 2 tapes.
 * - Returns tapes heads to beginnings.
 * - Perform merge writing onto third tape.
 * - Copy data from third tape to vector.
 * - Check result.
 */
TEST_P(MergeTapesTest, MergeSimple) {
  const auto& params = MergeTapesTest::GetParam();

  const auto tape0name = params.description + "_input_tape_0";
  const auto tape1name = params.description + "_input_tape_1";
  const auto outTapeName = params.description + "_output_tape";

  const bool tape0Removed = std::filesystem::remove(tape0name);
  const bool tape1Removed = std::filesystem::remove(tape1name);
  const bool outTapeRemoved = std::filesystem::remove(outTapeName);

  const bool anyFileWasRemoved = tape0Removed || tape1Removed || outTapeRemoved;

  // assert(!anyFileWasRemoved && "Some files were not removed.");

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

    inTape0.moveLeft(inSize0 - 1);
    inTape1.moveLeft(inSize1 - 1);

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

    std::vector<int> expected;

    if (params.increasing) {
      std::merge(params.input0.begin(), params.input0.end(),
                 params.input1.begin(), params.input1.end(),
                 std::back_inserter(expected));
      EXPECT_EQ(expected.size(), resultVec.size());
      EXPECT_TRUE(
          std::equal(expected.begin(), expected.end(), resultVec.begin()));
    } else {
      std::merge(params.input0.rbegin(), params.input0.rend(),
                 params.input1.rbegin(), params.input1.rend(),
                 std::back_inserter(expected));
      EXPECT_EQ(expected.size(), resultVec.size());
      EXPECT_TRUE(
          std::equal(expected.rbegin(), expected.rend(), resultVec.begin()));
    }
  }

  std::filesystem::remove(tape0name);
  std::filesystem::remove(tape1name);
  std::filesystem::remove(outTapeName);
}

static auto simpleMergesInputs = std::vector<MergeTapesTestParam>{
    {"merge_one_elements_tapes_increasing", {42}, {57}, true},
    {"merge_one_elements_tapes_decreasing", {42}, {57}, false},
    {"merge_one_element_and_multiple_increasing", {33}, {12, 24, 37}, true},
    {"merge_one_element_and_multiple_decreasing", {33}, {37, 24, 12}, false},
    {"merge_intersecting_increasing", {1, 3, 4}, {2, 5, 6}, true},
    {"merge_intersecting_decreasing", {4, 3, 1}, {6, 5, 2}, false},
    {"merge_non_intersecting_increasing", {1, 2, 3}, {4, 5, 6}, true},
    {"merge_non_intersecting_decreasing", {3, 2, 1}, {6, 5, 4}, false}};

INSTANTIATE_TEST_SUITE_P(SimpleTapesMerges, MergeTapesTest,
                         testing::ValuesIn(simpleMergesInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.description;
                         });

}  // namespace

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers,
// cert-err58-cpp)
