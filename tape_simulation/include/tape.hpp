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

  //////////////////////////////////////////////////////////////////////////////
  /// \brief class LeftOutOfRange - out of range exception
  class RightOutOfRange : public std::out_of_range {
   private:
    RightOutOfRange(const std::string& filename, std::size_t position);
    static std::string generateMessage_(const std::string& filename,
                                        std::size_t position);
    friend class Tape;
  };

  //////////////////////////////////////////////////////////////////////////////
  /// \brief class LeftOutOfRange - out of range exception
  class LeftOutOfRange : public std::out_of_range {
   private:
    explicit LeftOutOfRange(const std::string& filename);
    static std::string generateMessage_(const std::string& filename);
    friend class Tape;
  };

 public:
  constexpr static auto cellSize = sizeof(std::uint32_t);

 public:
  /**
   * @brief Tape constructor. Tape is created from an existing file if size is
   * given and opens a file as an exiting tape.
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
  [[nodiscard]] std::size_t getPosition() const;

  /**
   * @brief Move head one cell left.
   */
  void moveLeft();

  /**
   * @brief Move head one cell right.
   */
  void moveRight();

  /**
   * @brief Get tape cells count.
   *
   * @return std::size_t tape size.
   */
  [[nodiscard]] std::size_t getSize() const;

 private:
  std::size_t position_{0};
  std::string filename_;
  std::size_t size_;
  std::fstream file_;
};

////////////////////////////////////////////////////////////////////////////////
inline std::size_t Tape::getPosition() const {
  return position_;
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t Tape::getSize() const {
  return size_;
}

#endif
