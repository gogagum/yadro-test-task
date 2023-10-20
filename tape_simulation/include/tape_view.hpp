#ifndef TAPE_SIMULATION_TAPE_VIEW_HPP
#define TAPE_SIMULATION_TAPE_VIEW_HPP

#include <cstdint>
#include <fstream>
#include <string>
#include <tape.hpp>

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
   */
  void moveLeft();

  /**
   * @brief Move head right and update pool statistics.
   */
  void moveRight();

 private:
  Tape* tape_;
  TapePool* owner_;
  std::size_t position_{0};
  std::size_t size_;
  std::fstream file_;

 private:
  friend class TapePool;
};

#endif
