#include <cassert>
#include <copy_n.hpp>
#include <filesystem>
#include <impl/merge_sort.hpp>
#include <merge.hpp>

////////////////////////////////////////////////////////////////////////////////
MergeSortImpl::MergeSortImpl(TapePool& tapePool, std::string_view inFilename,
                             std::string_view tmpDirectory)
    : tapePool_{&tapePool},
      elementsCnt_{tapePool_->getOrOpenTape(inFilename).getSize()},
      inFilename_{inFilename},
      tmpDirectoryName_{tmpDirectory} {
  if (!std::filesystem::exists(tmpDirectory)) {
    std::filesystem::create_directory(tmpDirectory);
    needToRemoveTmpDirectoryFlag_ = true;
  }
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSortImpl::calcCounts_(std::size_t blocksCnt0, std::size_t blocksCnt1,
                                std::size_t blockSize) const -> Counts_ {
  const auto blocksCnt = blocksCnt0 + blocksCnt1;
  if (blocksCnt % 2 == 0) {
    assert(blocksCnt0 == blocksCnt1);
    return {getElementsCnt_() - blocksCnt1 * blockSize, blocksCnt1 * blockSize};
  } else {
    assert(blocksCnt0 == blocksCnt1 + 1);
    return {blocksCnt0 * blockSize, getElementsCnt_() - blocksCnt0 * blockSize};
  }
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::checkStartPositions_(TapeView& in0, TapeView& in1,
                                         TapeView& out0, TapeView& out1,
                                         std::size_t inCnt0Expected,
                                         std::size_t inCnt1Expected) const {
  assert(in0.getPosition() + 1 == inCnt0Expected);
  assert(in1.getPosition() + 1 == inCnt1Expected);
  assert(out0.getPosition() == 0);
  assert(out1.getPosition() == 0);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::checkFinishPositions_(TapeView& in0, TapeView& in1,
                                          TapeView& out0, TapeView& out1,
                                          std::size_t outCnt0Expected,
                                          std::size_t outCnt1Expected) const {
  assert(in0.getPosition() == 0);
  assert(in1.getPosition() == 0);
  assert(out0.getPosition() + 1 == outCnt0Expected);
  assert(out1.getPosition() + 1 == outCnt1Expected);
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSortImpl::calcOperationBlocksCnts_(std::size_t blockSize) const
    -> OperationBlocksCnts_ {
  auto inBlocksCnt = getBlocksCnt_(blockSize);
  auto [inBlocksCnt0, inBlocksCnt1] = getBlocksCnts_(blockSize);
  auto outBlockCnt = getBlocksCnt_(blockSize * 2);
  auto [outBlockCnt0, outBlockCnt1] = getBlocksCnts_(blockSize * 2);
  return {inBlocksCnt, inBlocksCnt0, inBlocksCnt1,
          outBlockCnt, outBlockCnt0, outBlockCnt1};
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
  assert(blocksOut0 >= blocksOut1);
  assert(blocksOut1 + 1 >= blocksOut0);

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
std::pair<std::size_t, std::size_t>
MergeSortImpl::getIterationsCntAndMaxBlockSize_(
    std::size_t startBlockSize) const {
  assert(startBlockSize < getElementsCnt_() &&
         "Start block size must be smaller than sorted tape.");

  std::size_t iterationsCnt = 0;
  std::size_t blockSize = startBlockSize;
  for (; blockSize * 2 < getElementsCnt_(); blockSize *= 2, ++iterationsCnt) {
  }

  return std::make_pair(iterationsCnt, blockSize);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::makeInitialBlocks_(TapeView& in, TapeView& out0,
                                       TapeView& out1) const {
  const auto [to0Cnt, to1Cnt] = getBlocksCnts_(1);
  auto read = RightReadIterator(in);
  copy_n(read, to0Cnt, RightWriteIterator(out0));
  ++read;
  copy_n(read, to1Cnt, RightWriteIterator(out1));
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks0_(TapeView& in0, TapeView& in1, TapeView& out0,
                              TapeView& out1, std::size_t blockSize) const {
  const auto opBlocksCnt = calcOperationBlocksCnts_(blockSize);

  const auto [inCnt0, inCnt1] =
      calcCounts_(opBlocksCnt.blocksIn0, opBlocksCnt.blocksIn1, blockSize);

  checkStartPositions_(in0, in1, out0, out1, inCnt0, inCnt1);

  auto read0 = LeftReadIterator(in0);
  auto read1 = LeftReadIterator(in1);

  auto write0 = RightWriteIterator(out0);
  auto write1 = RightWriteIterator(out1);

  auto& tailWrite = (opBlocksCnt.blocksOut % 2 == 0) ? write0 : write1;

  auto [inTailSize0, inTailSize1] =
      calcTailsCounts_(inCnt0, inCnt1, opBlocksCnt.blocksIn1, blockSize);

  if (inTailSize0 + inTailSize1 != 0) {
    processPartialBlocks_(read0, inTailSize0, read1, inTailSize1, tailWrite,
                          true, true);
  }

  processBlocksPairs_(read0, read1, write0, opBlocksCnt.blocksOut0, write1,
                      opBlocksCnt.blocksOut1, blockSize, true);

  const auto [outCnt0, outCnt1] = calcCounts_(
      opBlocksCnt.blocksOut0, opBlocksCnt.blocksOut1, blockSize * 2);

  checkFinishPositions_(in0, in1, out0, out1, outCnt0, outCnt1);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks1_(TapeView& in0, TapeView& in1, TapeView& out0,
                                  TapeView& out1, std::size_t blockSize) const {
  const auto opBlocksCnt = calcOperationBlocksCnts_(blockSize);

  const auto [inCnt0, inCnt1] =
      calcCounts_(opBlocksCnt.blocksIn0, opBlocksCnt.blocksIn1, blockSize);

  checkStartPositions_(in0, in1, out0, out1, inCnt0, inCnt1);

  auto read0 = LeftReadIterator(in0);
  auto read1 = LeftReadIterator(in1);

  auto write0 = RightWriteIterator(out0);
  auto write1 = RightWriteIterator(out1);

  processBlocksPairs_(read0, read1, write0, opBlocksCnt.blocksOut0, write1,
                      opBlocksCnt.blocksOut1, blockSize, false);

  auto& tailWrite = (opBlocksCnt.blocksOut % 2 == 0) ? write0 : write1;

  auto [inTailSize0, inTailSize1] =
      calcTailsCounts_(inCnt0, inCnt1, opBlocksCnt.blocksIn1, blockSize);

  if (inTailSize0 != 0 && opBlocksCnt.blocksOut != 0) {
    ++read0;
  }

  if (inTailSize1 != 0 && opBlocksCnt.blocksOut != 0) {
    ++read1;
  }

  if (inTailSize0 != 0 || inTailSize1 != 0) {
    if (opBlocksCnt.blocksOut1 != 0) {
      ++tailWrite;
    }
    processPartialBlocks_(read0, inTailSize0, read1, inTailSize1, tailWrite,
                          false, false);
  }

  const auto [outCnt0, outCnt1] = calcCounts_(
      opBlocksCnt.blocksOut0, opBlocksCnt.blocksOut1, blockSize * 2);

  checkFinishPositions_(in0, in1, out0, out1, outCnt0, outCnt1);
}

