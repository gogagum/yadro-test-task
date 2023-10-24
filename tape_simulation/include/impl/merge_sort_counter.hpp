#ifndef TAPE_SIMULATION_IMPL_MERGE_SORT_COUNTER
#define TAPE_SIMULATION_IMPL_MERGE_SORT_COUNTER

#include <cstdint>

class MergeSortCounter {
 public:
  struct OperationBlocksCnts_ {
    std::size_t blocksIn;
    std::size_t blocksIn0;
    std::size_t blocksIn1;
    std::size_t blocksOut;
    std::size_t blocksOut0;
    std::size_t blocksOut1;
  };

  struct Counts_ {
    std::size_t cnt0;
    std::size_t cnt1;
  };

 public:
  MergeSortCounter(std::size_t elementsCnt, std::size_t initialBlockSize);

  [[nodiscard]] std::size_t calcIterationsCnt() const;

  [[nodiscard]] OperationBlocksCnts_ calcOperationBlocksCnts(
      std::size_t blockSize) const;

  [[nodiscard]] Counts_ getBlocksCnts(std::size_t blockSize) const;

  [[nodiscard]] Counts_ calcCounts(std::size_t blocksCnt0,
                                   std::size_t blocksCnt1,
                                   std::size_t blockSize) const;

  [[nodiscard]] Counts_ calcTailsCounts(std::size_t elementsCnt0,
                                        std::size_t elementsCnt1,
                                        std::size_t blocksCnt1,
                                        std::size_t blockSize) const;

  [[nodiscard]] std::size_t getMaxBlockSize() const;

  [[nodiscard]] std::size_t getIterationsCnt() const;

 private:
  std::size_t elementsCnt_;
  std::size_t initialBlockSize_;
  std::size_t iterationsCnt_;
  const std::size_t maxBlockSize_;
};

////////////////////////////////////////////////////////////////////////////////
inline auto MergeSortCounter::calcTailsCounts(std::size_t elementsCnt0,
                                              std::size_t elementsCnt1,
                                              std::size_t blocksCnt1,
                                              std::size_t blockSize) const
    -> Counts_ {
  return {elementsCnt0 - blocksCnt1 * blockSize,
          elementsCnt1 - blocksCnt1 * blockSize};
}

////////////////////////////////////////////////////////////////////////////////
inline auto MergeSortCounter::getBlocksCnts(std::size_t blockSize) const
    -> Counts_ {
  return {(elementsCnt_ / blockSize - 1) / 2 + 1,
          (elementsCnt_ / blockSize) / 2};
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t MergeSortCounter::getMaxBlockSize() const {
  return maxBlockSize_;
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t MergeSortCounter::getIterationsCnt() const {
  return iterationsCnt_;
}

#endif  // TAPE_SIMULATION_IMPL_MERGE_SORT_COUNTER