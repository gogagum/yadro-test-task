#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <copy_elements_sorted.hpp>
#include <random>
#include <set>
#include <vector>

// NOLINTBEGIN(cert-err58-cpp,
// cppcoreguidelines-avoid-non-const-global-variables)

////////////////////////////////////////////////////////////////////////////////
TEST(CopyElementsSorted, CopyZero) {
  auto src = std::array<int, 3>{1, 2, 3};
  auto target = std::vector<int>{};

  const auto perform0 = [&]() {
    copy_top_elements_sorted(src.begin(), std::back_inserter(target), 0);
  };

  EXPECT_THROW(perform0(), std::logic_error);

  const auto perform1 = [&]() {
    copy_bottom_elements_sorted(src.begin(), std::back_inserter(target), 0);
  };

  EXPECT_THROW(perform1(), std::logic_error);
}

////////////////////////////////////////////////////////////////////////////////
TEST(CopyElementsSorted, Top2Elements) {
  auto src = std::array<int, 3>{1, 2, 3};
  auto target = std::vector<int>{};
  auto expected = std::array<int, 2>{2, 3};

  copy_top_elements_sorted(src.begin(), std::back_inserter(target), 2, 1);

  EXPECT_EQ(target.size(), 2);
  EXPECT_TRUE(std::equal(expected.begin(), expected.end(), target.begin()));
}

////////////////////////////////////////////////////////////////////////////////
TEST(CopyElementsSorted, Bottom2Elements) {
  auto src = std::array<int, 3>{1, 2, 3};
  auto target = std::vector<int>{};
  auto expected = std::array<int, 2>{2, 1};

  copy_bottom_elements_sorted(src.begin(), std::back_inserter(target), 2, 1);

  EXPECT_EQ(target.size(), 2);
  EXPECT_TRUE(std::equal(expected.begin(), expected.end(), target.begin()));
}

////////////////////////////////////////////////////////////////////////////////
TEST(CopyElementsSorted, SortTrivial) {
  auto src = std::array<int, 3>{1, 2, 3};
  auto target = std::vector<int>{};
  auto expected = std::array<int, 3>{1, 2, 3};

  copy_top_elements_sorted(src.begin(), std::back_inserter(target), 3, 0);

  EXPECT_EQ(target.size(), 3);
  EXPECT_TRUE(std::equal(expected.begin(), expected.end(), target.begin()));
}

////////////////////////////////////////////////////////////////////////////////
TEST(CopyElementsSorted, SortShuffled) {
  auto src = std::array<int, 6>{5, 3, 6, 2, 3, 1};
  auto target = std::vector<int>{};
  auto expected = src;
  std::sort(expected.begin(), expected.end());

  copy_top_elements_sorted(src.begin(), std::back_inserter(target), src.size(),
                           0);

  EXPECT_EQ(target.size(), 6);
  EXPECT_TRUE(std::equal(expected.begin(), expected.end(), target.begin()));
}

////////////////////////////////////////////////////////////////////////////////
TEST(CopyElementsSorted, FuzzTop) {
  auto gen = std::mt19937(42);

  for (std::size_t i = 0; i < 100; ++i) {
    std::vector<int> source;
    for (std::size_t j = 0; j < 100; ++j) {
      source.push_back(gen());
    }
    const std::size_t copiedCnt = gen() % 100;

    std::vector<int> target;

    copy_top_elements_sorted(source.begin(), std::back_inserter(target),
                             copiedCnt, 100 - copiedCnt);

    EXPECT_EQ(target.size(), copiedCnt);
    EXPECT_TRUE(std::is_sorted(target.begin(), target.end()));
  }
}

////////////////////////////////////////////////////////////////////////////////
TEST(CopyElementsSorted, FuzzBottom) {
  auto gen = std::mt19937(42);

  for (std::size_t i = 0; i < 100; ++i) {
    std::vector<int> source;
    for (std::size_t j = 0; j < 100; ++j) {
      source.push_back(gen());
    }
    const std::size_t copiedCnt = gen() % 100;

    std::vector<int> target;

    copy_bottom_elements_sorted(source.begin(), std::back_inserter(target),
                             copiedCnt, 100 - copiedCnt);

    EXPECT_EQ(target.size(), copiedCnt);
    EXPECT_TRUE(std::is_sorted(target.rbegin(), target.rend()));
  }
}

// NOLINTEND(cert-err58-cpp,
// cppcoreguidelines-avoid-non-const-global-variables)
