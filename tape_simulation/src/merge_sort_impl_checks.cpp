#include <cassert>
#include <impl/merge_sort.hpp>

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::checkStartPositions_(
    TapeView& in0, TapeView& in1, TapeView& out0, TapeView& out1,
    std::size_t blockSize) const {
  const auto opBlocksCnt = mergeSortCounter_.calcOperationBlocksCnts(blockSize);
  const auto [inCnt0Expected, inCnt1Expected] = mergeSortCounter_.calcCounts(
      opBlocksCnt.blocksIn0, opBlocksCnt.blocksIn1, blockSize);
  assert(in0.getPosition() + 1 == inCnt0Expected);
  assert(in1.getPosition() + 1 == inCnt1Expected);
  assert(out0.getPosition() == 0);
  assert(out1.getPosition() == 0);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::checkFinishPositions_(
    TapeView& in0, TapeView& in1, TapeView& out0, TapeView& out1,
    std::size_t blockSize) const {
  const auto opBlocksCnt = mergeSortCounter_.calcOperationBlocksCnts(blockSize);
  const auto [outCnt0Expected, outCnt1Expected] = mergeSortCounter_.calcCounts(
      opBlocksCnt.blocksOut0, opBlocksCnt.blocksOut1, blockSize * 2);
  assert(in0.getPosition() == 0);
  assert(in1.getPosition() == 0);
  assert(out0.getPosition() + 1 == outCnt0Expected);
  assert(out1.getPosition() + 1 == outCnt1Expected);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::checkFinalPositions_(const TapeView& in0,
                                         const TapeView& in1) const {
  assert(in0.getPosition() + 1 == mergeSortCounter_.getMaxBlockSize());
  assert(in1.getPosition() + 1 ==
         elementsCnt_ - mergeSortCounter_.getMaxBlockSize());
}
