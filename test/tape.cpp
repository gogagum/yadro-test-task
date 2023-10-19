#include <gtest/gtest.h>

#include <cassert>
#include <filesystem>
#include <tape.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)

TEST(Tape, ConstructNew) {
  constexpr auto filename = "new_constructed_tape";
  assert(!std::filesystem::remove(filename) &&
         "File was not removed in previous test run.");
  {
    auto tape =
        Tape(filename,
             std::fstream(filename, std::ios_base::in | std::ios_base::out |
                                        std::ios::binary | std::ios::app),
             4);
    EXPECT_EQ(std::filesystem::file_size(filename), 16);
    EXPECT_EQ(tape.getSize(), 4);
  }
  EXPECT_TRUE(std::filesystem::exists(filename));
  std::filesystem::remove(filename);
}

TEST(Tape, OpenFromFile) {
  constexpr auto filename = "open_from_file";
  assert(!std::filesystem::remove(filename) &&
         "File was not removed in previous test run.");

  std::fstream(filename, std::ios_base::in | std::ios_base::out |
                             std::ios::binary | std::ios::app);
  std::filesystem::resize_file(filename, 8);

  {
    auto tape =
        Tape(filename,
             std::fstream(filename, std::ios_base::in | std::ios_base::out |
                                        std::ios::binary | std::ios::app));
    EXPECT_EQ(tape.getSize(), 2);
  }
  EXPECT_TRUE(std::filesystem::exists(filename));
  std::filesystem::remove(filename);
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp)
