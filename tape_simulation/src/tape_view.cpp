#include <filesystem>
#include <sstream>
#include <tape_view.hpp>
#include <tape_view_fabric.hpp>

////////////////////////////////////////////////////////////////////////////////
TapeView::TapeView(TapeViewFabric& fabric, std::fstream&& file,
                   std::size_t size)
    : owner_{&fabric}, file_{std::move(file)}, size_{size} {
}

////////////////////////////////////////////////////////////////////////////////
std::int32_t TapeView::read() {
  auto ret = std::int32_t{};
  file_.seekg(static_cast<std::ptrdiff_t>(position_ * sizeof(std::uint32_t)));
  file_ >> ret;
  return ret;
  owner_->increaseReadsCnt();
}

////////////////////////////////////////////////////////////////////////////////
void TapeView::write(std::int32_t x) {
  file_.seekp(static_cast<ptrdiff_t>(position_ * sizeof(std::uint32_t)));
  file_ << x;
  owner_->increaseWritesCnt();
}

////////////////////////////////////////////////////////////////////////////////
void TapeView::moveLeft() {
  if (position_ == 0) {
    throw std::logic_error("Tried moving left from zero position.");
  }
  --position_;
  owner_->increaseMovesCnt();
}

////////////////////////////////////////////////////////////////////////////////
void TapeView::moveRight() {
  if (position_ == size_ - 1) {
    throw std::logic_error("Tried moving right from last position.");
  }
  ++position_;
  owner_->increaseMovesCnt();
}

////////////////////////////////////////////////////////////////////////////////
TapeView TapeView::createNew_(TapeViewFabric& fabric, std::string_view filename,
                              std::size_t size) {
  if (std::filesystem::exists(filename)) {
    std::stringstream message;
    message << "Trying creating a tape (" << filename
            << ") which already exists";
    throw std::logic_error(message.str());
  }
  auto fileStream = std::fstream(filename.data(),
                                 std::ios_base::binary | std::ios_base::in |
                                     std::ios_base::out | std::ios_base::trunc);
  return TapeView(fabric, std::move(fileStream), size);
}

////////////////////////////////////////////////////////////////////////////////
TapeView TapeView::open_(TapeViewFabric& fabric, std::string_view filename) {
  if (!std::filesystem::exists(filename)) {
    std::stringstream message;
    message << "Trying opening a tape (" << filename
            << ") which does not exist.";
    throw std::logic_error(message.str());
  }
  auto fileStream =
      std::fstream(filename.data(), std::ios::binary | std::ios::in |
                                        std::ios::out | std::ios::app);
  const auto size = static_cast<std::size_t>(fileStream.tellg()) / 4;
  fileStream.seekg(0);
  fileStream.seekp(0);
  return TapeView(fabric, std::move(fileStream), size);
}
