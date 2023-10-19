#include <cassert>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <tape_pool.hpp>

////////////////////////////////////////////////////////////////////////////////
TapeView TapePool::openTape(std::string_view filename) {
  increaseOpenCnt();
  if (tapes_.find(filename) != tapes_.end()) {
    std::stringstream messageStream;
    messageStream << "Trying opening tape(" << filename << ") twice.";
    throw std::logic_error(messageStream.str());
  }
  auto file =
      std::fstream(filename.data(), std::ios_base::in | std::ios_base::out |
                                        std::ios::binary | std::ios::app);
  tapes_.emplace(filename, Tape(filename, std::move(file)));
  return TapeView(*this, tapes_.at(filename));
}

////////////////////////////////////////////////////////////////////////////////
TapeView TapePool::createTape(std::string_view filename,
                                    std::size_t size) {
  increaseCreateCnt();
  if (tapes_.find(filename) != tapes_.end()) {
    std::stringstream messageStream;
    messageStream << "Trying creating a tape(" << filename
                  << ") which is already opened.";
    throw std::logic_error(messageStream.str());
  }
  if (std::filesystem::exists(filename)) {
    std::stringstream messageStream;
    messageStream << "Trying creating a tape(" << filename
                  << ") with filename which already exists.";
    throw std::logic_error(messageStream.str());
  }
  auto file = std::fstream(filename.data(),
                           std::ios_base::in | std::ios_base::out |
                               std::ios_base::binary | std::ios_base::trunc);
  tapes_.emplace(filename, Tape(filename, std::move(file), size));
  return TapeView(*this, tapes_.at(filename));
}

////////////////////////////////////////////////////////////////////////////////
void TapePool::removeTape(std::string_view filename) {
  increaseRemoveCnt();
  tapes_.erase(filename);
  std::filesystem::remove(filename);
}
