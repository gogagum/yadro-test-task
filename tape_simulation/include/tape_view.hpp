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
  explicit TapeView(TapePool& owner, Tape& tape);

 public:
  TapeView(TapeView&& other) noexcept = default;

  TapeView(const TapeView& other) = delete;

  TapeView& operator=(TapeView&& other) noexcept = default;

  TapeView& operator=(const TapeView& other) = delete;

  ~TapeView() = default;
  
  std::int32_t read();

  void write(std::int32_t x);

  void moveLeft();

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
