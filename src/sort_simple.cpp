#include <argparse/argparse.hpp>
#include <iostream>
#include <merge_sort.hpp>
#include <tape_pool.hpp>

#include "base_app.hpp"
#include "config_parser.hpp"

class SortSimple : BaseApp {
 public:
  SortSimple(int argc, const char* const* argv) : BaseApp(argc, argv) {
  }
  int run() && {
    parser_.add_argument("--in").required();
    parser_.add_argument("--out").required();
    parser_.add_argument("--config").required();

    try {
      parser_.parse_args(argc_, argv_);
    } catch (const std::runtime_error& e) {
      std::cerr << "Invalid arguments. " << e.what() << std::endl;
      return 1;
    }

    try {
      const auto inFilename = parser_.get("--in");
      const auto outFilename = parser_.get("--out");
      const auto configFilename = parser_.get("--config");

      auto tapePool = TapePool();
      auto config = ConfigParser(configFilename).read();
      MergeSort(tapePool, inFilename, "tmp", true).perform(outFilename);
      printReport_(config, tapePool.getStatistics());
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }

    return 0;
  }

 private:
  void printReport_(const ConfigParser::Config& config,
                    const TapePool::IOStatistics& ioStats) {
    std::cout << "Read count:\t" << ioStats.readCnt << std::endl;
    std::cout << "Write count:\t" << ioStats.writeCnt << std::endl;
    std::cout << "Move count:\t" << ioStats.moveCnt << std::endl;
    std::cout << "Create count:\t" << ioStats.createCnt << std::endl;
    std::cout << "Open count:\t" << ioStats.openCnt << std::endl;
    std::cout << "Close count:\t" << ioStats.closeCnt << std::endl;
    std::cout << "Remove count:\t" << ioStats.removeCnt << std::endl;
    std::cout << "Read time:\t" << ioStats.readCnt * config.readTime
              << std::endl;
    std::cout << "Write time:\t" << ioStats.writeCnt * config.writeTime
              << std::endl;
    std::cout << "Move time:\t" << ioStats.moveCnt * config.moveTime
              << std::endl;
    std::cout << "Create time:\t" << ioStats.createCnt * config.createTime
              << std::endl;
    std::cout << "Open time:\t" << ioStats.openCnt * config.openTime
              << std::endl;
    std::cout << "Close time:\t" << ioStats.closeCnt * config.closeTime
              << std::endl;
    std::cout << "Remove time:\t" << ioStats.removeCnt * config.removeTime
              << std::endl;
  }

 private:
  argparse::ArgumentParser parser_{};
};

int main(int argc, char* argv[]) {
  return SortSimple(argc, argv).run();
}
