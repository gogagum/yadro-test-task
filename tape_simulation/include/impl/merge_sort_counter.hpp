#ifndef TAPE_SIMULATION_IMPL_MERGE_SORT_COUNTER
#define TAPE_SIMULATION_IMPL_MERGE_SORT_COUNTER

#include <cstdint>

class MergeSortArithmeticsBase {
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

 protected:
  MergeSortArithmeticsBase(std::size_t elementsCnt,
                           std::size_t initialBlockSize);

  [[nodiscard]] OperationBlocksCnts_ calcOperationBlocksCnts_(
      std::size_t blockSize) const;

  [[nodiscard]] Counts_ getBlocksCnts_(std::size_t blockSize) const;

  [[nodiscard]] Counts_ calcCounts_(std::size_t blocksCnt0,
                                    std::size_t blocksCnt1,
                                    std::size_t blockSize) const;

  [[nodiscard]] Counts_ calcTailsCounts_(std::size_t blocksCnt1,
                                         std::size_t blockSize) const;

 private:
  [[nodiscard]] std::size_t calcIterationsCnt_() const;

 protected:
  const std::size_t elementsCnt_;
  const std::size_t initialBlockSize_;
  const std::size_t iterationsCnt_;
  const std::size_t maxBlockSize_;
};

////////////////////////////////////////////////////////////////////////////////
inline auto MergeSortArithmeticsBase::getBlocksCnts_(
    std::size_t blockSize) const -> Counts_ {
  return {(elementsCnt_ / blockSize - 1) / 2 + 1,
          (elementsCnt_ / blockSize) / 2};
}

#endif  // TAPE_SIMULATION_IMPL_MERGE_SORT_COUNTER