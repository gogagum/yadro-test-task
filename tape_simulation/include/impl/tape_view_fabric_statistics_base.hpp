#ifndef TAPE_SIMULATION_TAPE_VIEW_FABRIC_STATISTICS_BASE_HPP
#define TAPE_SIMULATION_TAPE_VIEW_FABRIC_STATISTICS_BASE_HPP

#include <cstdint>

class TapeViewFabricStatisticsBase {
 public:
  struct IOStatistics {
    std::size_t readCnt;
    std::size_t writeCnt;
    std::size_t moveCnt;
    std::size_t createCnt;
    std::size_t openCnt;
    std::size_t closeCnt;
    std::size_t removeCnt;
  };

 protected:
  TapeViewFabricStatisticsBase() = default;

 public:
  void increaseReadsCnt();

  void increaseWritesCnt();

  void increaseMovesCnt();

  void increaseCreateCnt();

  void increaseOpenCnt();

  void increaseCloseCnt();

  void increaseRemoveCnt();

  [[nodiscard]] IOStatistics getStatistics() const;

 private:
  IOStatistics statistics_{};
};

////////////////////////////////////////////////////////////////////////////////
inline void TapeViewFabricStatisticsBase::increaseReadsCnt() {
  ++statistics_.readCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapeViewFabricStatisticsBase::increaseWritesCnt() {
  ++statistics_.writeCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapeViewFabricStatisticsBase::increaseMovesCnt() {
  ++statistics_.moveCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapeViewFabricStatisticsBase::increaseCreateCnt() {
  ++statistics_.readCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapeViewFabricStatisticsBase::increaseOpenCnt() {
  ++statistics_.writeCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapeViewFabricStatisticsBase::increaseCloseCnt() {
  ++statistics_.moveCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapeViewFabricStatisticsBase::increaseRemoveCnt() {
  ++statistics_.moveCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline auto TapeViewFabricStatisticsBase::getStatistics() const
    -> IOStatistics {
  return statistics_;
}

#endif
