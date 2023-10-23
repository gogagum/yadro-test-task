#include <gtest/gtest.h>

#include <filesystem>
#include <merge_sort.hpp>
#include <vector>

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-avoid-magic-numbers, cert-err58-cpp)

namespace {

struct MergeSortTestParam {
  std::string testDescription;
  std::vector<std::int32_t> values;
};

class MergeSortTest : public testing::TestWithParam<MergeSortTestParam> {};

TEST_P(MergeSortTest, MergeSortSimpl) {
  const auto& params = MergeSortTest::GetParam();
  const auto inFilename = params.testDescription + "_in_file";
  const auto outFilename = params.testDescription + "_out_file";

  const bool inDeleted = std::filesystem::remove(inFilename);
  const bool outDeleted = std::filesystem::remove(outFilename);

  std::filesystem::remove_all("tmp");

  //assert(!(inDeleted || outDeleted) && "File was not removed.");

  {
    auto tapePool = TapePool();
    auto inTape = tapePool.createTape(inFilename, params.values.size());
    copy_n(params.values.begin(), params.values.size(),
           RightWriteIterator(inTape));
    inTape.moveLeft(inTape.getPosition());

    MergeSort(tapePool, inFilename, "tmp").perform(outFilename);

    auto outTape = tapePool.getOpenedTape(outFilename);
    outTape.moveLeft(outTape.getPosition());

    auto result = std::vector<std::int32_t>{};

    copy_n(RightReadIterator(outTape), params.values.size(),
           std::back_inserter(result));

    auto expected = std::vector<std::int32_t>(params.values);
    std::sort(expected.begin(), expected.end());

    EXPECT_EQ(expected.size(), result.size());
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), result.begin()));
  }

  std::filesystem::remove(inFilename);
  std::filesystem::remove(outFilename);
}

static auto simpleMergeSortInputs = std::vector<MergeSortTestParam>{
    {"sort_only_element", {42}},
    {"sort_two_sorted_elements", {42, 57}},
    {"sort_two_unsorted_elements", {57, 42}},
    {"sort_two_equal_elements", {42, 42}},
    {"sort_three_elements", {1, 3, 2}},
    {"sort_three_equal", {1, 1, 1}},
    {"sort_three_sorted", {1, 2, 3}},
    {"sort_four", {2, 3, 1, 4}},
    {"sort_four_sorted", {1, 2, 3, 4}},
    {"sort_four_decreasing", {4, 3, 2, 1}},
    {"sort_five", {5, 3, 2, 1, 4}},
    {"sort_five2", {5, 1, 2, 4, 3}},
    {"sort_five_sorted", {1, 2, 3, 4, 5}},
    {"sort_five_decreasing", {5, 4, 3, 2, 1}},
    {"sort_10", {9, 4, 5, 3, 1, 2, 7, 8, 6, 10}}
};

INSTANTIATE_TEST_SUITE_P(SimpleTapesMergeSorts, MergeSortTest,
                         testing::ValuesIn(simpleMergeSortInputs),
                         [](const auto& paramInfo) {
                           return paramInfo.param.testDescription;
                         });

}  // namespace

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-avoid-magic-numbers, cert-err58-cpp)
