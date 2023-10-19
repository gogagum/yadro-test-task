#ifndef TAPE_SIMULATION_I_TAPE
#define TAPE_SIMULATION_I_TAPE

#include <fstream>
#include <cstdint>

class ITape {
 public:
  ITape() = default;
  ITape(const ITape&) = delete;
  ITape(ITape&&) noexcept = default;
  ITape& operator=(const ITape&) = delete;
  ITape& operator=(ITape&&) noexcept = delete;
  [[nodiscard]] virtual std::size_t getPosition() const = 0;
  [[nodiscard]] virtual std::int32_t read() = 0;
  virtual void write(std::int32_t x) = 0;
  virtual void moveLeft() = 0;
  virtual void moveRight() = 0;
  virtual ~ITape() = default;
};

#endif