#include <copy_n.hpp>
#include <impl/merge_sort.hpp>
#include <merge.hpp>

////////////////////////////////////////////////////////////////////////////////
MergeSortImpl::MergeSortImpl(TapePool& tapePool, std::string_view inFilename,
                             std::string_view tmpDirectory,
                             std::size_t initialBlockSize, bool increasing)
    : tapePool_{&tapePool},
      elementsCnt_{tapePool_->getOrOpenTape(std::string(inFilename)).getSize()},
      mergeSortCounter_(elementsCnt_, initialBlockSize),
      inFilename_{inFilename},
      increasing_{increasing},
      tapesManager_(tapePool, tmpDirectory,
                    mergeSortCounter_.getMaxBlockSize()) {
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::processPartialBlocks_(
    LeftReadIterator& in0, std::size_t cnt0, LeftReadIterator& in1,
    std::size_t cnt1, RightWriteIterator& out0, bool increasing,
    bool moveInTheEnd) const {
  if (cnt1 == 0) {
    copy_n(in0, cnt0, out0);
    if (moveInTheEnd) {
      ++in0;
      ++out0;
    }
  } else {
    if (increasing) {
      merge_increasing(in0, cnt0, in1, cnt1, out0);
    } else {
      merge_decreasing(in0, cnt0, in1, cnt1, out0);
    }
    if (moveInTheEnd) {
      ++in0;
      ++in1;
      ++out0;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::processBlocksPairs_(
    LeftReadIterator& in0, LeftReadIterator& in1, RightWriteIterator& out0,
    std::size_t blocksOut0, RightWriteIterator& out1, std::size_t blocksOut1,
    std::size_t blockSize, bool increasing) const {
  for (std::size_t i = 0; i < blocksOut0; ++i) {
    if (increasing) {
      merge_increasing(in0, blockSize, in1, blockSize, out0);
    } else {
      merge_decreasing(in0, blockSize, in1, blockSize, out0);
    }
    if (i + 1 != blocksOut0) {
      ++in0;
      ++in1;
      ++out0;
    }
  }

  if (blocksOut1 != 0) {
    ++in0;
    ++in1;
    for (std::size_t i = 0; i < blocksOut1; ++i) {
      if (increasing) {
        merge_increasing(in0, blockSize, in1, blockSize, out1);
      } else {
        merge_decreasing(in0, blockSize, in1, blockSize, out1);
      }
      if (i + 1 != blocksOut1) {
        ++in0;
        ++in1;
        ++out1;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks_(TapeView& in0, TapeView& in1, TapeView& out0,
                                 TapeView& out1, std::size_t blockSize,
                                 std::size_t iterationsLeft) const {
  if (iterationsLeft % 2 == 1) {
    mergeBlocks1_(in0, in1, out0, out1, blockSize);
  } else {
    mergeBlocks0_(in0, in1, out0, out1, blockSize);
  }
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks0_(TapeView& in0, TapeView& in1, TapeView& out0,
                                  TapeView& out1, std::size_t blockSize) const {
  const auto opBlocksCnt = mergeSortCounter_.calcOperationBlocksCnts(blockSize);

  const auto [inCnt0, inCnt1] = mergeSortCounter_.calcCounts(
      opBlocksCnt.blocksIn0, opBlocksCnt.blocksIn1, blockSize);

  checkStartPositions_(in0, in1, out0, out1, inCnt0, inCnt1);

  auto read0 = LeftReadIterator(in0);
  auto read1 = LeftReadIterator(in1);

  auto write0 = RightWriteIterator(out0);
  auto write1 = RightWriteIterator(out1);

  auto& tailWrite = (opBlocksCnt.blocksOut % 2 == 0) ? write0 : write1;

  auto [inTailSize0, inTailSize1] = mergeSortCounter_.calcTailsCounts(
      inCnt0, inCnt1, opBlocksCnt.blocksIn1, blockSize);

  if (inTailSize0 + inTailSize1 != 0) {
    processPartialBlocks_(read0, inTailSize0, read1, inTailSize1, tailWrite,
                          increasing_, true);
  }

  processBlocksPairs_(read0, read1, write0, opBlocksCnt.blocksOut0, write1,
                      opBlocksCnt.blocksOut1, blockSize, increasing_);

  checkFinishPositions_(in0, in1, out0, out1, opBlocksCnt, blockSize);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks1_(TapeView& in0, TapeView& in1, TapeView& out0,
                                  TapeView& out1, std::size_t blockSize) const {
  const auto opBlocksCnt = mergeSortCounter_.calcOperationBlocksCnts(blockSize);

  const auto [inCnt0, inCnt1] = mergeSortCounter_.calcCounts(
      opBlocksCnt.blocksIn0, opBlocksCnt.blocksIn1, blockSize);

  checkStartPositions_(in0, in1, out0, out1, inCnt0, inCnt1);

  auto read0 = LeftReadIterator(in0);
  auto read1 = LeftReadIterator(in1);

  auto write0 = RightWriteIterator(out0);
  auto write1 = RightWriteIterator(out1);

  processBlocksPairs_(read0, read1, write0, opBlocksCnt.blocksOut0, write1,
                      opBlocksCnt.blocksOut1, blockSize, !increasing_);

  auto [inTailSize0, inTailSize1] = mergeSortCounter_.calcTailsCounts(
      inCnt0, inCnt1, opBlocksCnt.blocksIn1, blockSize);

  if (inTailSize0 != 0 && opBlocksCnt.blocksOut != 0) {
    ++read0;
  }

  if (inTailSize1 != 0 && opBlocksCnt.blocksOut != 0) {
    ++read1;
  }

  auto& tailWrite = (opBlocksCnt.blocksOut % 2 == 0) ? write0 : write1;

  if (inTailSize0 != 0 || inTailSize1 != 0) {
    if (opBlocksCnt.blocksOut1 != 0) {
      ++tailWrite;
    }
    processPartialBlocks_(read0, inTailSize0, read1, inTailSize1, tailWrite,
                          !increasing_, false);
  }

  checkFinishPositions_(in0, in1, out0, out1, opBlocksCnt, blockSize);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeIntoOutputTape_(TapeView& inTape0, TapeView& inTape1,
                                         TapeView& outTape) const {
  checkFinalPositions_(inTape0, inTape1);

  auto in0 = LeftReadIterator(inTape0);
  auto in1 = LeftReadIterator(inTape1);
  auto out = RightWriteIterator(outTape);

  const auto blockSize = mergeSortCounter_.getMaxBlockSize();

  if (increasing_) {
    merge_increasing(in0, blockSize, in1, elementsCnt_ - blockSize, out);
  } else {
    merge_decreasing(in0, blockSize, in1, elementsCnt_ - blockSize, out);
  }
}
