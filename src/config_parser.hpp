#include <fstream>
#include <sstream>
#include <string_view>

class ConfigParser {
 public:
  struct Config {
    std::size_t moveTime;
    std::size_t readTime;
    std::size_t writeTime;
    std::size_t openTime;
    std::size_t createTime;
    std::size_t closeTime;
    std::size_t removeTime;
  };

 public:
  explicit ConfigParser(std::string_view filename) : txtFile_(filename.data()) {
  }
  Config read() && {
    auto ret = Config{readSizeT_(), readSizeT_(), readSizeT_(), readSizeT_(),
                      readSizeT_(), readSizeT_(), readSizeT_()};
    if (std::string anything; static_cast<bool>(txtFile_ >> anything)) {
      throw std::logic_error("Invalid config file. To much parameters.");
    }
    return ret;
  }

 private:
  std::size_t readSizeT_() {
    auto ret = std::size_t{};
    if (!static_cast<bool>(txtFile_ >> ret)) {
      throw std::logic_error("Invalid config file. Too few parameters.");
    }
    return ret;
  }

 public:
  std::ifstream txtFile_;
};