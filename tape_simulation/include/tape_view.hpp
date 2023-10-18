#ifndef TAPE_SIMULATION_TAPE_VIEW_HPP
#define TAPE_SIMULATION_TAPE_VIEW_HPP

#include <cstdint>
#include <fstream>
#include <string>

class TapeViewFabric;

class TapeView {
 private:
  explicit TapeView(TapeViewFabric& fabricPtr, std::fstream&& file,
                    std::size_t fileSize);

 public:
  std::int32_t read();

  void write(std::int32_t x);

  void moveLeft();

  void moveRight();

 private:
  static TapeView createNew_(TapeViewFabric& fabric,
                             std::string_view, std::size_t size);

  static TapeView open_(TapeViewFabric& fabric, const std::string& filename);

 private:
  TapeViewFabric* owner_;
  std::size_t position_{0};
  std::size_t size_;
  std::fstream file_;

 private:
  friend class TapeViewFabric;
};

#endif
