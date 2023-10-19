#ifndef TAPE_SIMULATION_TAPE_HPP
#define TAPE_SIMULATION_TAPE_HPP

#include <cstdint>
#include <fstream>
#include <optional>
#include <string>

////////////////////////////////////////////////////////////////////////////////
/// \brief class Tape - tape modelling over a file.
class Tape {
 public:
  constexpr static auto cellSize = sizeof(std::uint32_t);

 public:
  Tape(std::string_view filename, std::fstream&& file,
       std::optional<std::size_t> size = std::nullopt);
  Tape(Tape&&) noexcept = default;
  Tape(const Tape&) = delete;
  Tape& operator=(Tape&&) noexcept = delete;
  Tape& operator=(const Tape&) = delete;
  ~Tape() = default;

  [[nodiscard]] std::int32_t read();

  void write(std::int32_t x);

  [[nodiscard]] std::size_t getPosition() const;

  void moveLeft();

  void moveRight();

  [[nodiscard]] std::size_t getSize() const;
  
 private:
  std::size_t position_{0};
  std::string filename_;
  std::size_t size_;
  std::fstream file_;
};

////////////////////////////////////////////////////////////////////////////////
inline std::size_t Tape::getSize() const {
  return size_;
}

#endif
