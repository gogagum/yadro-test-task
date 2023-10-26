#include <cassert>
#include <filesystem>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <tape.hpp>

////////////////////////////////////////////////////////////////////////////////
Tape::RightOutOfRange::RightOutOfRange(const std::string& filename,
                                       std::size_t position)
    : std::out_of_range(generateMessage_(filename, position)) {
}

////////////////////////////////////////////////////////////////////////////////
std::string Tape::RightOutOfRange::generateMessage_(const std::string& filename,
                                                    std::size_t position) {
  std::stringstream messageStream;
  messageStream << "Trying moving right from the most right position ("
                << position << ") in tape \"" << filename << "\".";
  return messageStream.str();
}

////////////////////////////////////////////////////////////////////////////////
Tape::LeftOutOfRange::LeftOutOfRange(const std::string& filename)
    : std::out_of_range(generateMessage_(filename)) {
}

////////////////////////////////////////////////////////////////////////////////
std::string Tape::LeftOutOfRange::generateMessage_(
    const std::string& filename) {
  std::stringstream messageStream;
  messageStream
      << "Trying moving left from the most left position (0) in tape \""
      << filename << "\".";
  return messageStream.str();
}

////////////////////////////////////////////////////////////////////////////////
Tape::Tape(std::string_view filename, std::optional<std::size_t> size)
    : filename_{filename},
      size_{size.has_value() ? *size
                             : std::filesystem::file_size(filename) / 4},
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
  file_.read(reinterpret_cast<char*>(&ret), cellSize);  // NOLINT
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
void Tape::write(std::int32_t x) {
  file_.seekp(static_cast<std::ptrdiff_t>(position_ * cellSize));
  file_.write(reinterpret_cast<const char*>(&x), cellSize);  // NOLINT
}

////////////////////////////////////////////////////////////////////////////////
void Tape::moveLeft() {
  if (position_ == 0) {
    throw LeftOutOfRange(filename_);
  }
  --position_;
}

////////////////////////////////////////////////////////////////////////////////
void Tape::moveRight() {
  if (position_ + 1 == size_) {
    throw RightOutOfRange(filename_, position_);
  }
  ++position_;
}
