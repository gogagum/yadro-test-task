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
  /**
   * @brief Tape constructor. Tape is created from an existing file if size is
   * given an opens a file as an exiting tape.
   *
   * @param filename tape filename.
   * @param size size of a tape.
   */
  explicit Tape(std::string_view filename,
                std::optional<std::size_t> size = std::nullopt);
  Tape(Tape&&) noexcept = default;
  Tape(const Tape&) = delete;
  Tape& operator=(Tape&&) noexcept = delete;
  Tape& operator=(const Tape&) = delete;
  ~Tape() = default;

  /**
   * @brief read cell.
   * 
   * @return std::int32_t read value.
   */
  [[nodiscard]] std::int32_t read();

  /**
   * @brief write to a current cell.
   * 
   * @param x value to write.
   */
  void write(std::int32_t x);

  /**
   * @brief get current head position.
   * 
   * @return std::size_t head position, index of a cell.
   */
  [[nodiscard]] std::int64_t getPosition() const;

  /**
   * @brief Move head one cell left.
   * 
   * @param i move count.
   */
  void moveLeft(std::size_t i = 1);

  /**
   * @brief Move head one cell right.
   * 
   * @param i move count.
   */
  void moveRight(std::size_t i = 1);

  /**
   * @brief Get tape cells count.
   * 
   * @return std::size_t tape size.
   */
  [[nodiscard]] std::size_t getSize() const;

 private:
  std::int64_t position_{0};
  std::string filename_;
  std::size_t size_;
  std::fstream file_;
};

////////////////////////////////////////////////////////////////////////////////
inline std::int64_t Tape::getPosition() const {
  return position_;
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t Tape::getSize() const {
  return size_;
}

#endif
