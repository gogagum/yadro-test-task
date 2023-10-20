#include <gtest/gtest.h>

#include <cassert>
#include <filesystem>
#include <tape.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)

////////////////////////////////////////////////////////////////////////////////
TEST(Tape, ConstructNew) {
  constexpr auto filename = "new_constructed_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not removed in previous test run.");
  {
    auto tape = Tape(filename, 4);
    EXPECT_EQ(std::filesystem::file_size(filename), 16);
    EXPECT_EQ(tape.getSize(), 4);
  }
  EXPECT_TRUE(std::filesystem::exists(filename));
  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Tape, OpenFromFile) {
  constexpr auto filename = "open_from_file";
  assert(!std::filesystem::remove(filename) &&
         "File was not removed in previous test run.");

  std::fstream(filename, std::ios_base::in | std::ios_base::out |
                             std::ios::binary | std::ios::app);
  std::filesystem::resize_file(filename, 8);

  {
    auto tape = Tape(filename);
    EXPECT_EQ(tape.getSize(), 2);
  }
  EXPECT_TRUE(std::filesystem::exists(filename));
  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Tape, MovingLeftOnceDoesNotBreak) {
  constexpr auto filename = "moving_left_once_does_not_break";
  assert(!std::filesystem::remove(filename) &&
         "File was not removed in previous test run.");

  {
    auto tape = Tape(filename, 2);
    tape.moveLeft();
  }
  EXPECT_TRUE(std::filesystem::exists(filename));
  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Tape, MovingRightOnceAfterLastDoesNotBreak) {
  constexpr auto filename = "moving_right_once_after_last_does_not_break";
  assert(!std::filesystem::remove(filename) &&
         "File was not removed in previous test run.");

  {
    auto tape = Tape(filename, 2);
    tape.moveRight();
    tape.moveRight();
  }
  EXPECT_TRUE(std::filesystem::exists(filename));
  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Tape, MovingLeftOnceAndReadingDoesBreak) {
  constexpr auto filename = "moving_left_once_and_reading_does_break";
  assert(!std::filesystem::remove(filename) &&
         "File was not removed in previous test run.");

  {
    auto tape = Tape(filename, 2);
    tape.moveLeft();
    EXPECT_THROW(auto x = tape.read(), std::logic_error);
  }
  EXPECT_TRUE(std::filesystem::exists(filename));
  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Tape, MovingRightOnceAfterLastAndReadingDoesBreak) {
  constexpr auto filename = "moving_right_once_after_last_and_reading_does_break";
  assert(!std::filesystem::remove(filename) &&
         "File was not removed in previous test run.");

  {
    auto tape = Tape(filename, 2);
    tape.moveRight();
    tape.moveRight();
    EXPECT_THROW(auto x = tape.read(), std::logic_error);
  }
  EXPECT_TRUE(std::filesystem::exists(filename));
  std::filesystem::remove(filename);
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)
