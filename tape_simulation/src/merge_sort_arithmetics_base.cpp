#include <impl/merge_sort_arithmetics_base.hpp>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
MergeSortArithmeticsBase::ZeroInitialBlockSize_::ZeroInitialBlockSize_()
    : std::invalid_argument(
          "Trying creating MergeSortArithmeticBase with zero initial block "
          "size.") {
}

////////////////////////////////////////////////////////////////////////////////
MergeSortArithmeticsBase::MergeSortArithmeticsBase(std::size_t elementsCnt,
                                                   std::size_t initialBlockSize)
    : elementsCnt_{elementsCnt},
      initialBlockSize_{initialBlockSize},
      iterationsCnt_{calcIterationsCnt_()},
      maxBlockSize_{initialBlockSize_ << iterationsCnt_} {
}

////////////////////////////////////////////////////////////////////////////////
std::size_t MergeSortArithmeticsBase::calcIterationsCnt_() const {
  if (initialBlockSize_ == 0) {
    throw ZeroInitialBlockSize_();
  }
  
  std::size_t iterationsCnt = 0;

  for (std::size_t blockSize = initialBlockSize_; blockSize * 2 < elementsCnt_;
       blockSize *= 2, ++iterationsCnt) {
  }

  return iterationsCnt;
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSortArithmeticsBase::calcTailsCounts_(std::size_t blocksCnt1,
                                                std::size_t blockSize) const
    -> Counts_ {
  const auto opBlocksCnt = calcOperationBlocksCnts_(blockSize);
  const auto [elementsCnt0, elementsCnt1] =
      calcCounts_(opBlocksCnt.in0, opBlocksCnt.in1, blockSize);
  return {elementsCnt0 - blocksCnt1 * blockSize,
          elementsCnt1 - blocksCnt1 * blockSize};
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSortArithmeticsBase::calcOperationBlocksCnts_(
    std::size_t blockSize) const -> OperationBlocksCnts_ {
  const auto inBlocksCnt = elementsCnt_ / blockSize;
  const auto [inBlocksCnt0, inBlocksCnt1] = getBlocksCnts_(blockSize);
  const auto outBlockCnt = elementsCnt_ / (blockSize * 2);
  const auto [outBlockCnt0, outBlockCnt1] = getBlocksCnts_(blockSize * 2);
  return {inBlocksCnt, inBlocksCnt0, inBlocksCnt1,
          outBlockCnt, outBlockCnt0, outBlockCnt1};
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSortArithmeticsBase::calcCounts_(std::size_t blocksCnt0,
                                           std::size_t blocksCnt1,
                                           std::size_t blockSize) const
    -> Counts_ {
  const auto blocksCnt = blocksCnt0 + blocksCnt1;
  if (blocksCnt % 2 == 0) {
    return {elementsCnt_ - blocksCnt1 * blockSize, blocksCnt1 * blockSize};
  } else {
    return {blocksCnt0 * blockSize, elementsCnt_ - blocksCnt0 * blockSize};
  }
}
