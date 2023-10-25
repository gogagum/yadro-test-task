#ifndef TAPE_SIMULATION_IMPL_MERGE_SORT_ARITHMETICS_BASE_HPP
#define TAPE_SIMULATION_IMPL_MERGE_SORT_ARITHMETICS_BASE_HPP

#include <cstdint>

class MergeSortArithmeticsBase {
 protected:
  struct OperationBlocksCnts_ {
    std::size_t in;
    std::size_t in0;
    std::size_t in1;
    std::size_t out;
    std::size_t out0;
    std::size_t out1;
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

#endif  // TAPE_SIMULATION_IMPL_MERGE_SORT_ARITHMETICS_BASE_HPP