#ifndef TAPE_SIMULATION_TAPE_POOL_STATISTICS_BASE_HPP
#define TAPE_SIMULATION_TAPE_POOL_STATISTICS_BASE_HPP

#include <cstdint>

class TapePoolStatisticsBase {
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
  TapePoolStatisticsBase() = default;

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
inline void TapePoolStatisticsBase::increaseReadsCnt() {
  ++statistics_.readCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapePoolStatisticsBase::increaseWritesCnt() {
  ++statistics_.writeCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapePoolStatisticsBase::increaseMovesCnt() {
  ++statistics_.moveCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapePoolStatisticsBase::increaseCreateCnt() {
  ++statistics_.createCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapePoolStatisticsBase::increaseOpenCnt() {
  ++statistics_.openCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapePoolStatisticsBase::increaseCloseCnt() {
  ++statistics_.closeCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline void TapePoolStatisticsBase::increaseRemoveCnt() {
  ++statistics_.moveCnt;
}

////////////////////////////////////////////////////////////////////////////////
inline auto TapePoolStatisticsBase::getStatistics() const
    -> IOStatistics {
  return statistics_;
}

#endif
