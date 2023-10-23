#ifndef TAPE_SIMULATION_TAPE_VIEW_HPP
#define TAPE_SIMULATION_TAPE_VIEW_HPP

#include <cstdint>
#include <fstream>
#include <string>

#include "tape.hpp"

class TapePool;

////////////////////////////////////////////////////////////////////////////////
/// \brief class TapeView non-owning class for tapes control.
class TapeView {
 private:
  /**
   * @brief Constructor for calling inside TapePool.
   *
   * @param owner pool.
   * @param tape tape controlled.
   */
  explicit TapeView(TapePool& owner, Tape& tape);

 public:
  TapeView(TapeView&& other) noexcept = default;

  TapeView(const TapeView& other) = delete;

  TapeView& operator=(TapeView&& other) noexcept = default;

  TapeView& operator=(const TapeView& other) = delete;

  ~TapeView() = default;

  /**
   * @brief Read current tape and update pool statistics.
   *
   * @return std::int32_t value read.
   */
  std::int32_t read();

  /**
   * @brief Write to a current tape and update pool statistics.
   *
   * @param x value to write.
   */
  void write(std::int32_t x);

  /**
   * @brief Move head left and update pool statistics.
   *
   * @param i moves count.
   */
  void moveLeft(std::size_t i = 1);

  /**
   * @brief Move head right and update pool statistics.
   *
   * @param i moves count.
   */
  void moveRight(std::size_t = 1);

  /**
   * @brief Get size of the tape.
   *
   * @return tape cells count
   */
  [[nodiscard]] std::size_t getSize() const;

  /**
   * @brief Get head position.
   *
   * @return get current tape cell.
   */
  [[nodiscard]] std::int64_t getPosition() const;

 private:
  Tape* tape_;
  TapePool* owner_;
  std::size_t size_;
  std::fstream file_;

 private:
  friend class TapePool;
};

////////////////////////////////////////////////////////////////////////////////
inline std::size_t TapeView::getSize() const {
  return tape_->getSize();
}

////////////////////////////////////////////////////////////////////////////////
inline std::int64_t TapeView::getPosition() const {
  return tape_->getPosition();
}

#endif
