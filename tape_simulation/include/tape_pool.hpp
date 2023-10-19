#ifndef TAPE_SIMULATION_TAPE_VIEW_FABRIC_HPP
#define TAPE_SIMULATION_TAPE_VIEW_FABRIC_HPP

#include <cstdint>
#include <map>
#include <string>
#include <tape_view.hpp>
#include <tape.hpp>

#include "impl/tape_view_fabric_statistics_base.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief class TapePool - tape views fabric, which counts operations.
class TapePool : public TapeViewFabricStatisticsBase {
 public:
  using IOStatistics = TapeViewFabricStatisticsBase::IOStatistics;

 public:
  TapePool() = default;

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
