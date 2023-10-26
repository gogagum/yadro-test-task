#include <argparse/argparse.hpp>
#include <cstdint>
#include <iostream>
#include <string>
#include <tape_pool.hpp>
#include "base_app.hpp"

class WriteFromConsole : BaseApp {
 public:
  WriteFromConsole(int argc, const char* const* argv) : BaseApp(argc, argv) {}
  int run() && {
    parser_.add_argument("--out").required();
    parser_.add_argument("--size").required();

    try {
      parser_.parse_args(argc_, argv_);
    } catch (const std::runtime_error& e) {
      std::cerr << "Invalid arguments. " << e.what() << std::endl;
      return 1;
    }

    auto tapePool = TapePool();

    try {
      std::stringstream sizeStream(parser_.get("--size"));
      auto numbersCount = std::size_t{};
      sizeStream >> numbersCount;
      auto tapeToWrite =
          tapePool.createTape(parser_.get("--out"), numbersCount);

      for (std::size_t i = 0; i < numbersCount; ++i) {
        auto number = std::int32_t{};
        std::cin >> number;

        tapeToWrite.write(number);

        if (i + 1 != numbersCount) {
          tapeToWrite.moveRight();
        }
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }

    return 0;
  }

 private:
  argparse::ArgumentParser parser_{};
};

int main(int argc, char* argv[]) {
  return WriteFromConsole(argc, argv).run();
}