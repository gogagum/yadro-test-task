#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <merge.hpp>
#include <random>

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeTwoEmptyIncreasing) {
  const auto src = std::array{1, 2, 3};
  auto target = std::vector<std::uint32_t>{};

  merge_increasing(src.begin(), 0, src.begin(), 0, std::back_inserter(target));

  EXPECT_EQ(target.size(), 0);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeTwoEmptyDecreasing) {
  const auto src = std::array{1, 2, 3};
  auto target = std::vector<std::uint32_t>{};

  merge_decreasing(src.begin(), 0, src.begin(), 0, std::back_inserter(target));

  EXPECT_EQ(target.size(), 0);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeIncreasingWhileOneIsEmpty) {
  const auto src = std::array{1, 2, 3};
  auto target1 = std::vector<std::uint32_t>{};
  auto target2 = std::vector<std::uint32_t>{};

  const auto expected = std::array{1, 2, 3};

  merge_increasing(src.begin(), 3, src.begin(), 0, std::back_inserter(target1));
  EXPECT_EQ(target1.size(), 3);
  EXPECT_TRUE(std::equal(target1.begin(), target1.end(), expected.begin()));

  merge_increasing(src.begin(), 0, src.begin(), 3, std::back_inserter(target2));
  EXPECT_EQ(target2.size(), 3);
  EXPECT_TRUE(std::equal(target2.begin(), target2.end(), expected.begin()));
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, MergeDecreasingWhileOneIsEmpty) {
  const auto src = std::array{3, 2, 1};
  auto target1 = std::vector<std::uint32_t>{};
  auto target2 = std::vector<std::uint32_t>{};

  const auto expected = std::array{3, 2, 1};

  merge_decreasing(src.begin(), 3, src.begin(), 0, std::back_inserter(target1));
  EXPECT_EQ(target1.size(), 3);
  EXPECT_TRUE(std::equal(target1.begin(), target1.end(), expected.begin()));

  merge_decreasing(src.begin(), 0, src.begin(), 3, std::back_inserter(target2));
  EXPECT_EQ(target2.size(), 3);
  EXPECT_TRUE(std::equal(target2.begin(), target2.end(), expected.begin()));
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

    merge_increasing(seq1.begin(), seq1.size(), seq2.begin(), seq2.size(),
                     std::back_inserter(result));

    EXPECT_EQ(expected.size(), result.size());
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), result.begin()));
  }
}

////////////////////////////////////////////////////////////////////////////////
TEST(Merge, FuzzDecreasing) {
  auto gen = std::mt19937(42);

  for (std::size_t i = 0; i < 100; ++i) {
    auto cnt1 = std::size_t{gen() % 100};
    auto cnt2 = std::size_t{gen() % 100};

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

    auto expected = std::vector<std::int32_t>{};
    expected.reserve(cnt1 + cnt2);

    std::merge(seq1.rbegin(), seq1.rend(), seq2.rbegin(), seq2.rend(),
               std::back_inserter(expected));

    auto result = std::vector<std::int32_t>{};
    result.reserve(cnt1 + cnt2);

    merge_decreasing(seq1.begin(), seq1.size(), seq2.begin(), seq2.size(),
                     std::back_inserter(result));

    EXPECT_EQ(expected.size(), result.size());
    EXPECT_TRUE(std::equal(expected.rbegin(), expected.rend(), result.begin()));
  }
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)
