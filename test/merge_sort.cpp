#include <gtest/gtest.h>

#include <filesystem>
#include <merge_sort.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-avoid-magic-numbers, cert-err58-cpp)

////////////////////////////////////////////////////////////////////////////////
TEST(MergeSort, DoNothing) {
  constexpr auto filename = "do_nothing_sort_test";
  assert(!std::filesystem::remove(filename) && "File was not removed.");
  auto tapePool = TapePool();
  tapePool.createTape(filename, 10);

  auto sort = MergeSort(tapePool, filename, "tmp");

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(MergeSort, SortEmpty) {
  constexpr auto filename = "sort_empty_test";
  constexpr auto outFilename = "sort_empty_test_out";
  assert(!std::filesystem::remove(filename) && "File was not removed.");
  assert(!std::filesystem::remove(outFilename) && "File was not removed.");

  {
    auto tapePool = TapePool();
    tapePool.createTape(filename, 0);

    MergeSort(tapePool, filename, "tmp").perform(outFilename);
  }

  std::filesystem::remove(filename);
  std::filesystem::remove(outFilename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(MergeSort, SortOneElement) {
  constexpr auto filename = "sort_one_element_test";
  constexpr auto outFilename = "sort_one_element_test_out";

  assert(!std::filesystem::remove(filename) && "File was not removed.");
  assert(!std::filesystem::remove(outFilename) && "File was not removed.");

  {
    auto tapePool = TapePool();
    auto tv = tapePool.createTape(filename, 1);
    tv.write(42);

    MergeSort(tapePool, filename, "tmp").perform(outFilename);

    auto outTv = tapePool.getOpenedTape(outFilename);
    EXPECT_EQ(outTv.getPosition(), 1);
    outTv.moveLeft();
    EXPECT_EQ(outTv.read(), 42);
  }

  std::filesystem::remove(filename);
  std::filesystem::remove(outFilename);
}

////////////////////////////////////////////////////////////////////////////////
/*
TEST(MergeSort, SortTwoSorted) {
  constexpr auto filename = "sort_two_sorted_test";
  constexpr auto outFilename = "sort_two_sorted_test_out";

  std::filesystem::remove(filename);
  std::filesystem::remove(outFilename);
  std::filesystem::remove_all("tmp");

  //assert(!std::filesystem::remove(filename) && "File was not removed.");
  //assert(!std::filesystem::remove(outFilename) && "File was not removed.");

  {
    auto tapePool = TapePool();
    auto tv = tapePool.createTape(filename, 2);
    tv.write(42);
    tv.moveRight();
    tv.write(57);
    tv.moveLeft();

    MergeSort(tapePool, filename, "tmp").perform(outFilename);

    auto outTv = tapePool.getOpenedTape(outFilename);
    EXPECT_EQ(outTv.getPosition(), 2);
    outTv.moveLeft();
    EXPECT_EQ(outTv.read(), 57);
    outTv.moveLeft();
    EXPECT_EQ(outTv.read(), 42);
  }

  std::filesystem::remove(filename);
  std::filesystem::remove(outFilename);
}
*/


// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-avoid-magic-numbers, cert-err58-cpp)
