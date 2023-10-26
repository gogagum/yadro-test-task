#include <argparse/argparse.hpp>
#include <cstdint>
#include <random>
#include <tape.hpp>

#include "base_app.hpp"

class GenerateTape : BaseApp {
 public:
  GenerateTape(int argc, const char* const* argv) : BaseApp(argc, argv) {}
  int run() && {
    parser_.add_argument("--out").required();
    parser_.add_argument("--size").required();
    parser_.add_argument("--seed").default_value("42");

    try {
      parser_.parse_args(argc_, argv_);
    } catch (const std::runtime_error& e) {
      std::cerr << "Invalid arguments. " << e.what() << std::endl;
      return 1;
    }

    try {
      const std::string filename = parser_.get("--out");
      auto size = std::size_t{};
      std::stringstream sizeStream(parser_.get("--size"));
      sizeStream >> size;

      auto seed = std::int32_t{};
      std::stringstream seedStream(parser_.get("--seed"));
      seedStream >> seed;

      auto gen = std::mt19937(seed);

      auto tape = Tape(filename, size);

      for (std::size_t i = 0; i < size; ++i) {
        tape.write(static_cast<std::int32_t>(gen()));
        if (i+1 != size) {
          tape.moveRight();
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
  return GenerateTape(argc, argv).run();
}
