#ifndef TAPE_SIMULATION_TAPE_VIEW_FABRIC_HPP
#define TAPE_SIMULATION_TAPE_VIEW_FABRIC_HPP

#include <cstdint>
#include <map>
#include <string>
#include <tape_view.hpp>

#include "impl/tape_view_fabric_statistics_base.hpp"
#include "impl/i_tape.hpp"

class TapeViewFabric : public TapeViewFabricStatisticsBase {
 private:
  class Tape : public ITape {
   public:
    constexpr static auto cellSize = sizeof(std::uint32_t);

   public:
    Tape(std::string_view filename, std::size_t size, std::fstream&& file);
    Tape(Tape&&) noexcept = default;
    Tape(const Tape&) = delete;
    ~Tape() override = default;
    Tape& operator=(Tape&&) noexcept = delete;
    Tape& operator=(const Tape&) = delete;

    [[nodiscard]] std::int32_t read() override;

    void write(std::int32_t x) override;

    std::size_t getPosition() const override;

    void moveLeft() override;

    void moveRight() override;

   private:
    std::size_t position_{0};
    std::string filename_;
    std::size_t size_;
    std::fstream file_;
  };

 public:
  using IOStatistics = TapeViewFabricStatisticsBase::IOStatistics;

 public:
  TapeViewFabric() = default;

  [[nodiscard]] TapeView openTape(std::string_view filename);

  [[nodiscard]] TapeView createTape(std::string_view filename,
                                    std::size_t size);

  void removeTape(std::string_view filename);

 private:
  std::fstream createTape_(std::string_view filename);

 private:
  std::map<std::string_view, Tape> tapes_;

 private:
  friend class TapeView;
};

#endif
