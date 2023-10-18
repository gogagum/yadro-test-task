#ifndef TAPE_SIMULATION_TAPE_VIEW_FABRIC_HPP
#define TAPE_SIMULATION_TAPE_VIEW_FABRIC_HPP

#include <map>
#include <string>
#include <tape_view.hpp>
#include "impl/tape_view_fabric_statistics_base.hpp"

class TapeViewFabric : public TapeViewFabricStatisticsBase {
 public:
  
  using IOStatistics = TapeViewFabricStatisticsBase::IOStatistics;

 public:
  TapeViewFabric() = default;

  [[nodiscard]] TapeView& openTape(std::string_view filename);

  [[nodiscard]] TapeView& createTape(std::string_view filename,
                                     std::size_t size);

  void removeTape(std::string_view filename);

 private:
  std::map<std::string_view, TapeView> openedViews_;
};

#endif
