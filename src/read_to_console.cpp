#include <argparse/argparse.hpp>
#include <iostream>
#include <string>
#include <tape_pool.hpp>
#include "base_app.hpp"

class ReadToConsole : BaseApp {
 public:
  ReadToConsole(int argc, const char* const* argv) : BaseApp(argc, argv) {}
  int run() && {
    parser_.add_argument("--in").required();

    try {
      parser_.parse_args(argc_, argv_);
    } catch (const std::runtime_error& e) {
      std::cerr << "Invalid arguments. " << e.what() << std::endl;
      return 1;
    }

    auto tapePool = TapePool();

    try {
      auto readTape = tapePool.openTape(parser_.get("--in"));
      std::cout << "Tape size: " << readTape.getSize() << std::endl;
      for (std::size_t i = 0; i < readTape.getSize(); ++i) {
        std::cout << readTape.read() << std::endl;
        if (i + 1 != readTape.getSize()) {
          readTape.moveRight();
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
  return ReadToConsole(argc, argv).run();
}
