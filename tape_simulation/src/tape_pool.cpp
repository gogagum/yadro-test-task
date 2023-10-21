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
  tapes_.emplace(filename, Tape(filename));
  return TapeView(*this, tapes_.at(filename));
}

////////////////////////////////////////////////////////////////////////////////
TapeView TapePool::createTape(std::string_view filename, std::size_t size) {
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
  tapes_.emplace(filename, Tape(filename, size));
  return TapeView(*this, tapes_.at(filename));
}

////////////////////////////////////////////////////////////////////////////////
TapeView TapePool::getOpenedTapeView(std::string_view filename) {
  if (tapes_.find(filename) == tapes_.end()) {
    std::stringstream messageStream;
    messageStream
        << "Trying getting a view to a tape that was not opened or created yet("
        << filename << ").";
    throw std::logic_error(messageStream.str());
  }
  return TapeView(*this, tapes_.at(filename));
}

////////////////////////////////////////////////////////////////////////////////
void TapePool::removeTape(std::string_view filename) {
  increaseRemoveCnt();
  tapes_.erase(filename);
  std::filesystem::remove(filename);
}
