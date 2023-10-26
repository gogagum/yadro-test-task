#include <cassert>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <tape_pool.hpp>

////////////////////////////////////////////////////////////////////////////////
TapeView TapePool::openTape(const std::string& filename) {
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
TapeView TapePool::createTape(const std::string& filename, std::size_t size) {
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
TapeView TapePool::getOpenedTape(const std::string& filename) {
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
TapeView TapePool::getOrOpenTape(const std::string& filename) {
  if (tapes_.find(filename) != tapes_.end()) {
    return TapeView(*this, tapes_.at(filename));
  }
  return openTape(filename);
}

////////////////////////////////////////////////////////////////////////////////
void TapePool::removeTape(const std::string& filename) {
  if (tapes_.find(filename) == tapes_.end()) {
    std::stringstream messageStream;
    messageStream << "Trying removing tape (" << filename
                  << ") which is not opened." << std::endl;
    throw std::logic_error(messageStream.str());
  }
  increaseRemoveCnt();
  tapes_.erase(filename);
  std::filesystem::remove(filename);
}

////////////////////////////////////////////////////////////////////////////////
void TapePool::closeTape(const std::string& filename) {
  if (tapes_.find(filename) == tapes_.end()) {
    std::stringstream messageStream;
    messageStream << "Trying closing tape (" << filename
                  << ") which is not opened." << std::endl;
    throw std::logic_error(messageStream.str());
  }
  increaseCloseCnt();
  tapes_.erase(filename);
}
