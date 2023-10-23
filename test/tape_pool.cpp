#include <gtest/gtest.h>

#include <filesystem>
#include <tape_pool.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)

////////////////////////////////////////////////////////////////////////////////
TEST(TapePool, Construct) {
  auto tapePool = TapePool();
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapePool, CreateTape) {
  constexpr auto filename = "create_test_tape";

  assert(!std::filesystem::exists(filename) &&
         "Tape file was not removed on previous tests run.");

  {
    auto tapePool = TapePool();
    auto tapeView = tapePool.createTape(filename, 4);

    const auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.moveCnt, 0);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapePool, CreateTapeAndMove) {
  constexpr auto filename = "create_test_tape_and_move";

  assert(!std::filesystem::remove(filename) &&
         "Tape file was not removed on previous tests run.");

  {
    auto tapePool = TapePool();
    auto tapeView = tapePool.createTape(filename, 4);

    tapeView.moveRight();
    tapeView.moveRight();
    
    const auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.moveCnt, 2);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapePool, CreateTapeAndMoveRightAndBack) {
  constexpr auto filename = "create_test_tape_and_move_right_and_back";

  assert(!std::filesystem::remove(filename) &&
         "Tape file was not removed on previous tests run.");

  {
    auto tapePool = TapePool();
    auto tapeView = tapePool.createTape(filename, 4);

    tapeView.moveRight();
    tapeView.moveLeft();
    
    const auto stats = tapePool.getStatistics();

    EXPECT_EQ(stats.createCnt, 1);
    EXPECT_EQ(stats.moveCnt, 2);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapePool, CreateTapeAndMoveThroughLeftBorder) {
  constexpr auto filename = "create_test_tape_and_move_through_left_border";

  assert(!std::filesystem::remove(filename) &&
         "Tape file was not removed on previous tests run.");

  {
    auto tapePool = TapePool();
    auto tapeView = tapePool.createTape(filename, 4);

    EXPECT_THROW(tapeView.moveLeft(), std::logic_error);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapePool, CreateTapeAndMoveThroughRightBorder) {
  constexpr auto filename = "create_test_tape_and_move_through_left_border";

  assert(!std::filesystem::exists(filename) &&
         "Tape file was not removed on previous tests run.");

  {
    auto tapePool = TapePool();
    auto tapeView = tapePool.createTape(filename, 4);

    tapeView.moveRight();
    tapeView.moveRight();
    tapeView.moveRight();
    EXPECT_THROW(tapeView.moveRight(), std::logic_error);
  }

  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(TapePool, CreateTapeReadAndWrite) {
  constexpr auto filename = "create_tape_write_and_read";

  assert(!std::filesystem::remove(filename) &&
         "Tape file was not removed on previous tests run.");
  
  {
    auto tapePool = TapePool();
    auto tapeView = tapePool.createTape(filename, 4);

    tapeView.write(42);
    
    EXPECT_EQ(tapeView.read(), 42);
  }

  std::filesystem::remove(filename);
}

TEST(TapePool, CreateTapeReadAndWriteMultiple) {
  constexpr auto filename = "create_tape_write_and_read_multiple";

  assert(!std::filesystem::remove(filename) &&
         "Tape file was not removed on previous tests run.");

  {
    auto tapePool = TapePool();
    auto tapeView = tapePool.createTape(filename, 4);

    tapeView.write(42);
    tapeView.moveRight();
    tapeView.write(37);
    tapeView.moveRight();
    tapeView.write(73);

    EXPECT_EQ(tapeView.read(), 73);
    tapeView.moveLeft();
    EXPECT_EQ(tapeView.read(), 37);
    tapeView.write(53);
    tapeView.moveLeft();
    EXPECT_EQ(tapeView.read(), 42);
    tapeView.moveRight();
    EXPECT_EQ(tapeView.read(), 53);
  }

  std::filesystem::remove(filename);
}

TEST(TapePool, WriteCloseOpenRead) {
  constexpr auto filename = "create_tape_write_and_read_multiple";

  assert(!std::filesystem::exists(filename) &&
         "Tape file was not removed on previous tests run.");

  {
    auto tapePool = TapePool();
    auto writingView = tapePool.createTape(filename, 4);

    writingView.write(42);
    writingView.moveRight();
    writingView.write(37);
    writingView.moveRight();
    writingView.write(73);
    writingView.moveRight();
    writingView.write(53);
  }

  {
    auto readingTapePool = TapePool();
    auto readingView = readingTapePool.openTape(filename);

    EXPECT_EQ(readingView.read(), 42);
    readingView.moveRight();
    EXPECT_EQ(readingView.read(), 37);
    readingView.moveRight();
    EXPECT_EQ(readingView.read(), 73);
    readingView.moveRight();
    EXPECT_EQ(readingView.read(), 53);
  }

  std::filesystem::remove(filename);
}

TEST(TapePool, RemoveTape) {
  constexpr auto filename = "deleted_tape";

  assert(!std::filesystem::remove(filename) &&
         "Tape file was not removed on previous tests run.");

  {
    auto tapePool = TapePool();
    auto deletedTape = tapePool.createTape(filename, 3);

    tapePool.removeTape(filename);
  }

  EXPECT_FALSE(std::filesystem::exists(filename));
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)
