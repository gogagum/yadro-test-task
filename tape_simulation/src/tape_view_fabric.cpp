#include <cassert>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <tape_view.hpp>
#include <tape_view_fabric.hpp>

////////////////////////////////////////////////////////////////////////////////
TapeViewFabric::Tape::Tape(std::string_view filename, std::size_t size,
                           std::fstream&& file)
    : filename_{filename}, size_{size * cellSize}, file_{std::move(file)} {
}

////////////////////////////////////////////////////////////////////////////////
std::int32_t TapeViewFabric::Tape::read() {
  auto ret = std::int32_t{};
  file_.seekg(static_cast<std::ptrdiff_t>(position_));
  file_ >> ret;
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
void TapeViewFabric::Tape::write(std::int32_t x) {
  file_.seekp(static_cast<std::ptrdiff_t>(position_));
  file_ << x;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t TapeViewFabric::Tape::getPosition() const {
  return position_;
}

////////////////////////////////////////////////////////////////////////////////
void TapeViewFabric::Tape::moveLeft() {
  assert(position_ % cellSize == 0 && "Invalid cell position,");
  if (position_ == 0) {
    std::stringstream messageStream;
    messageStream << "Trying moving left from the most left position in tape \""
                  << filename_ << "\".";
    throw std::logic_error(messageStream.str());
  }
  position_ -= cellSize;
}

////////////////////////////////////////////////////////////////////////////////
void TapeViewFabric::Tape::moveRight() {
  assert(position_ % cellSize == 0 && "Invalid cell position.");
  if (position_ == size_ - cellSize) {
    std::stringstream messageStream;
    messageStream
        << "Trying moving right from the most right position in tape \""
        << filename_ << "\".";
    throw std::logic_error(messageStream.str());
  }
  position_ += cellSize;
}

////////////////////////////////////////////////////////////////////////////////
TapeView TapeViewFabric::openTape(std::string_view filename) {
  increaseOpenCnt();
  if (tapes_.find(filename) != tapes_.end()) {
    std::stringstream messageStream;
    messageStream << "Trying opening tape(" << filename << ") twice.";
    throw std::logic_error(messageStream.str());
  }
  auto file =
      std::fstream(filename.data(), std::ios_base::in | std::ios_base::out |
                                        std::ios::binary | std::ios::app);
  std::size_t size = file.tellg();
  tapes_.emplace(filename, Tape(filename, size, std::move(file)));
  return TapeView(*this, tapes_.at(filename));
}

////////////////////////////////////////////////////////////////////////////////
TapeView TapeViewFabric::createTape(std::string_view filename,
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
  std::filesystem::resize_file(filename, size * 4);
  tapes_.emplace(filename, Tape(filename, size, std::move(file)));
  return TapeView(*this, tapes_.at(filename));
}

////////////////////////////////////////////////////////////////////////////////
void TapeViewFabric::removeTape(std::string_view filename) {
  increaseRemoveCnt();
  tapes_.erase(filename);
  std::filesystem::remove(filename);
}
