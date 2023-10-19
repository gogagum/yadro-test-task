#include <cassert>
#include <filesystem>
#include <optional>
#include <sstream>
#include <tape.hpp>

////////////////////////////////////////////////////////////////////////////////
Tape::Tape(std::string_view filename, std::optional<std::size_t> size)
    : filename_{filename},
      size_{size.has_value() ? *size : std::filesystem::file_size(filename) / 4},
      file_{std::fstream(
          filename.data(),
          std::ios_base::in | std::ios_base::out |
              (size.has_value() ? std::ios_base::trunc : std::ios_base::app) |
              std::ios_base::binary)} {
  if (size.has_value()) {
    std::filesystem::resize_file(filename_, *size * cellSize);
  }
}

////////////////////////////////////////////////////////////////////////////////
std::int32_t Tape::read() {
  auto ret = std::int32_t{};
  file_.seekg(static_cast<std::ptrdiff_t>(position_ * cellSize));
  file_ >> ret;
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
void Tape::write(std::int32_t x) {
  file_.seekp(static_cast<std::ptrdiff_t>(position_ * cellSize));
  file_ << x;
}

////////////////////////////////////////////////////////////////////////////////
void Tape::moveLeft() {
  if (position_ == 0) {
    std::stringstream messageStream;
    messageStream << "Trying moving left from the most left position in tape \""
                  << filename_ << "\".";
    throw std::logic_error(messageStream.str());
  }
  --position_;
}

////////////////////////////////////////////////////////////////////////////////
void Tape::moveRight() {
  if (position_ == size_ - 1) {
    std::stringstream messageStream;
    messageStream
        << "Trying moving right from the most right position in tape \""
        << filename_ << "\".";
    throw std::logic_error(messageStream.str());
  }
  ++position_;
}