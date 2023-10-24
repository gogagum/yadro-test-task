#include <impl/merge_sort_counter.hpp>

////////////////////////////////////////////////////////////////////////////////
MergeSortCounter::MergeSortCounter(std::size_t elementsCnt,
                                   std::size_t initialBlockSize)
    : elementsCnt_{elementsCnt},
      initialBlockSize_{initialBlockSize},
      iterationsCnt_{calcIterationsCnt()},
      maxBlockSize_{initialBlockSize_ << iterationsCnt_} {
}

////////////////////////////////////////////////////////////////////////////////
std::size_t MergeSortCounter::calcIterationsCnt() const {
  std::size_t iterationsCnt = 0;

  for (std::size_t blockSize = initialBlockSize_; blockSize * 2 < elementsCnt_;
       blockSize *= 2, ++iterationsCnt) {
  }

  return iterationsCnt;
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSortCounter::calcOperationBlocksCnts(std::size_t blockSize) const
    -> OperationBlocksCnts_ {
  auto inBlocksCnt = elementsCnt_ / blockSize;
  auto [inBlocksCnt0, inBlocksCnt1] = getBlocksCnts(blockSize);
  auto outBlockCnt = elementsCnt_ / (blockSize * 2);
  auto [outBlockCnt0, outBlockCnt1] = getBlocksCnts(blockSize * 2);
  return {inBlocksCnt, inBlocksCnt0, inBlocksCnt1,
          outBlockCnt, outBlockCnt0, outBlockCnt1};
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSortCounter::calcCounts(std::size_t blocksCnt0,
                                   std::size_t blocksCnt1,
                                   std::size_t blockSize) const -> Counts_ {
  const auto blocksCnt = blocksCnt0 + blocksCnt1;
  if (blocksCnt % 2 == 0) {
    return {elementsCnt_ - blocksCnt1 * blockSize, blocksCnt1 * blockSize};
  } else {
    return {blocksCnt0 * blockSize, elementsCnt_ - blocksCnt0 * blockSize};
  }
}
