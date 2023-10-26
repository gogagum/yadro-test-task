#include <gtest/gtest.h>

#include <cassert>
#include <filesystem>
#include <tape_pool.hpp>
#include <tape_view_write_iterators.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers,
// cert-err58-cpp)

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewWriteIterator, ConstructRight) {
  constexpr auto filename = "construct_right_iterator_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto writeIterator = RightWriteIterator(tapeView);
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
TEST(TapeViewWriteIterator, ConstructLeft) {
  constexpr auto filename = "construct_left_iterator_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto writeIterator = LeftWriteIterator(tapeView);
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
TEST(TapeViewWriteIterator, ConstructRightAndWrite) {
  constexpr auto filename = "construct_right_iterator_and_write_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto writeIterator = RightWriteIterator(tapeView);

      *writeIterator = 57;

      EXPECT_EQ(tapeView.read(), 57);
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
TEST(TapeViewWriteIterator, ConstructLeftAndWrite) {
  constexpr auto filename = "construct_left_iterator_and_write_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto writeIterator = LeftWriteIterator(tapeView);

      *writeIterator = 57;

      EXPECT_EQ(tapeView.read(), 57);
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
TEST(TapeViewWriteIterator, ConstructRightAndMove) {
  constexpr auto filename = "construct_right_iterator_and_move_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto writeIterator = RightWriteIterator(tapeView);

      ++writeIterator;
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
TEST(TapeViewWriteIterator, ConstructRightAndMovePostfix) {
  constexpr auto filename = "construct_right_iterator_and_move_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);
      auto writeIterator = RightWriteIterator(tapeView);

      writeIterator++;
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
TEST(TapeViewWriteIterator, ConstructLeftAndMove) {
  constexpr auto filename = "construct_left_iterator_and_move_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 42);

      auto writeIterator = LeftWriteIterator(tapeView);

      EXPECT_THROW(++writeIterator, std::logic_error);
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
TEST(TapeViewWriteIterator, ConstructLeftAndMoveOutOfRange) {
  constexpr auto filename =
      "construct_left_iterator_and_move_out_of_range_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 2);
      auto writeIterator = LeftWriteIterator(tapeView);
      tapeView.moveRight();
      ++writeIterator;

      EXPECT_THROW(++writeIterator, std::logic_error);
    }
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewWriteIterator, CopyRight) {
  constexpr auto filename = "copy_right_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 10);
      auto writeIterator = RightWriteIterator(tapeView);

      auto numbers = std::array<int, 5>{2, 3, 4, 2, 1};

      std::copy(numbers.begin(), numbers.end(), writeIterator);

      auto stats = tapePool.getStatistics();

      EXPECT_EQ(stats.createCnt, 1);
      EXPECT_EQ(stats.readCnt, 0);
      EXPECT_EQ(stats.writeCnt, 5);
      EXPECT_EQ(stats.moveCnt, 5);

      tapeView.moveLeft();
      EXPECT_EQ(tapeView.read(), 1);
      tapeView.moveLeft();
      EXPECT_EQ(tapeView.read(), 2);
      tapeView.moveLeft();
      EXPECT_EQ(tapeView.read(), 4);
      tapeView.moveLeft();
      EXPECT_EQ(tapeView.read(), 3);
      tapeView.moveLeft();
      EXPECT_EQ(tapeView.read(), 2);
    }
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapeViewWriteIterator, CopyLeft) {
  constexpr auto filename = "copy_right_test_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not deleted on previous run.");

  {
    auto tapePool = TapePool();

    {
      auto tapeView = tapePool.createTape(filename, 10);
      auto writeIterator = LeftWriteIterator(tapeView);

      for (std::size_t i = 0; i < 5; ++i) {
        tapeView.moveRight();
      }

      auto numbers = std::array<int, 5>{2, 3, 4, 2, 1};

      std::copy(numbers.begin(), numbers.end(), writeIterator);

      auto stats = tapePool.getStatistics();

      EXPECT_EQ(stats.createCnt, 1);
      EXPECT_EQ(stats.readCnt, 0);
      EXPECT_EQ(stats.writeCnt, 5);
      EXPECT_EQ(stats.moveCnt, 10);

      tapeView.moveRight();
      EXPECT_EQ(tapeView.read(), 1);
      tapeView.moveRight();
      EXPECT_EQ(tapeView.read(), 2);
      tapeView.moveRight();
      EXPECT_EQ(tapeView.read(), 4);
      tapeView.moveRight();
      EXPECT_EQ(tapeView.read(), 3);
      tapeView.moveRight();
      EXPECT_EQ(tapeView.read(), 2);
    }

    auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.readCnt, 5);
    EXPECT_EQ(stats.writeCnt, 5);
    EXPECT_EQ(stats.moveCnt, 15);
  }

  std::filesystem::remove(filename);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers,
// cert-err58-cpp)
