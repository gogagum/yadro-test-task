#include <gtest/gtest.h>

#include <cassert>
#include <filesystem>
#include <tape_pool.hpp>
#include <tape_view_read_iterators.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers,
// cert-err58-cpp)

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewReadIterator, ConstructRight) {
  constexpr auto filename = "construct_right_read_iterator_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto readIterator = RightReadIterator(tapeView);
    }

    auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.readCnt, 0);
    EXPECT_EQ(stats.writeCnt, 0);
    EXPECT_EQ(stats.moveCnt, 0);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewReadIterator, ConstructLeft) {
  constexpr auto filename = "construct_left_iterator_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto readIterator = LeftReadIterator(tapeView);
    }

    auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.readCnt, 0);
    EXPECT_EQ(stats.writeCnt, 0);
    EXPECT_EQ(stats.moveCnt, 0);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewReadIterator, ConstructRightAndRead) {
  constexpr auto filename = "construct_right_iterator_and_read_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      tapeView.write(42);
      auto readIterator = RightReadIterator(tapeView);
      EXPECT_EQ(*readIterator, 42);
    }

    auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.readCnt, 1);
    EXPECT_EQ(stats.writeCnt, 1);
    EXPECT_EQ(stats.moveCnt, 0);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewReadIterator, ConstructLeftAndRead) {
  constexpr auto filename = "construct_left_iterator_and_read_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      tapeView.write(42);
      auto readIterator = LeftReadIterator(tapeView);
      EXPECT_EQ(*readIterator, 42);
    }

    auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.readCnt, 1);
    EXPECT_EQ(stats.writeCnt, 1);
    EXPECT_EQ(stats.moveCnt, 0);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewReadIterator, ConstructRightAndMove) {
  constexpr auto filename = "construct_right_iterator_and_move_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto readIterator = RightReadIterator(tapeView);
      ++readIterator;
    }

    auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.readCnt, 0);
    EXPECT_EQ(stats.writeCnt, 0);
    EXPECT_EQ(stats.moveCnt, 1);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewReadIterator, ConstructLeftAndMove) {
  constexpr auto filename = "construct_left_iterator_and_move_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      tapeView.moveRight();
      auto readIterator = LeftReadIterator(tapeView);
      ++readIterator;
    }

    auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.readCnt, 0);
    EXPECT_EQ(stats.writeCnt, 0);
    EXPECT_EQ(stats.moveCnt, 2);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewReadIterator, RightEquality) {
  constexpr auto filename = "right_equality_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto readIterator1 = RightReadIterator(tapeView);
      auto readIterator2 = RightReadIterator(tapeView);
      EXPECT_TRUE(readIterator1 == readIterator2);
    }

    auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.readCnt, 0);
    EXPECT_EQ(stats.writeCnt, 0);
    EXPECT_EQ(stats.moveCnt, 0);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewReadIterator, LeftEquality) {
  constexpr auto filename = "left_equality_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto readIterator1 = LeftReadIterator(tapeView);
      auto readIterator2 = LeftReadIterator(tapeView);
      EXPECT_TRUE(readIterator1 == readIterator2);
    }

    auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.readCnt, 0);
    EXPECT_EQ(stats.writeCnt, 0);
    EXPECT_EQ(stats.moveCnt, 0);
  }

  std::filesystem::remove(filename);
}


// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers,
// cert-err58-cpp)
