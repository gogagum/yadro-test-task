#include <cassert>
#include <copy_n.hpp>
#include <filesystem>
#include <impl/merge_sort.hpp>
#include <merge.hpp>

////////////////////////////////////////////////////////////////////////////////
MergeSortImpl::MergeSortImpl(TapePool& tapePool, std::string_view inFilename,
                             std::string_view tmpDirectory,
                             std::size_t initialBlockSize)
    : tapePool_{&tapePool},
      elementsCnt_{tapePool_->getOrOpenTape(std::string(inFilename)).getSize()},
      initialBlockSize_{initialBlockSize},
      iterationsCnt_{calcIterationsCnt_()},
      maxBlockSize_{initialBlockSize_ << iterationsCnt_},
      inFilename_{inFilename},
      tmpDirectoryName_{tmpDirectory},
      needToRemoveTmpDirectory_{openOrCreateTmpPath_(tmpDirectory)},
      tmpTape00_{createTmpTape_("00")},
      tmpTape01_{createTmpTape_("01")},
      tmpTape10_{createTmpTape_("10")},
      tmpTape11_{createTmpTape_("11")} {
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSortImpl::calcCounts_(std::size_t blocksCnt0, std::size_t blocksCnt1,
                                std::size_t blockSize) const -> Counts_ {
  const auto blocksCnt = blocksCnt0 + blocksCnt1;
  if (blocksCnt % 2 == 0) {
    assert(blocksCnt0 == blocksCnt1);
    return {elementsCnt_ - blocksCnt1 * blockSize, blocksCnt1 * blockSize};
  } else {
    assert(blocksCnt0 == blocksCnt1 + 1);
    return {blocksCnt0 * blockSize, elementsCnt_ - blocksCnt0 * blockSize};
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
                                          const MergeSortImpl::OperationBlocksCnts_& opBlocksCnt,
                                          std::size_t blockSize) const {
  const auto [outCnt0Expected, outCnt1Expected] = calcCounts_(
      opBlocksCnt.blocksOut0, opBlocksCnt.blocksOut1, blockSize * 2);
  assert(in0.getPosition() == 0);
  assert(in1.getPosition() == 0);
  assert(out0.getPosition() + 1 == outCnt0Expected);
  assert(out1.getPosition() + 1 == outCnt1Expected);
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSortImpl::calcOperationBlocksCnts_(std::size_t blockSize) const
    -> OperationBlocksCnts_ {
  auto inBlocksCnt = elementsCnt_ / blockSize;
  auto [inBlocksCnt0, inBlocksCnt1] = getBlocksCnts_(blockSize);
  auto outBlockCnt = elementsCnt_ / (blockSize * 2);
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
std::size_t MergeSortImpl::calcIterationsCnt_() const {
  std::size_t iterationsCnt = 0;

  for (std::size_t blockSize = initialBlockSize_; blockSize * 2 < elementsCnt_;
       blockSize *= 2, ++iterationsCnt) {
  }

  return iterationsCnt;
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

  checkFinishPositions_(in0, in1, out0, out1, opBlocksCnt, blockSize);
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

  checkFinishPositions_(in0, in1, out0, out1, opBlocksCnt, blockSize);
}

////////////////////////////////////////////////////////////////////////////////
bool MergeSortImpl::openOrCreateTmpPath_(std::string_view tmpDirectory) {
  if (!std::filesystem::exists(tmpDirectory)) {
    std::filesystem::create_directory(tmpDirectory);
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////
TapeView MergeSortImpl::createTmpTape_(std::string_view nameSuffix) const {
  std::stringstream filenameStream;
  filenameStream << tmpDirectoryName_ << "/tmp_tape_" << nameSuffix;
  return tapePool_->createTape(filenameStream.str(), maxBlockSize_);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::checkFinalPositions_(const TapeView& in0,
                                         const TapeView& in1) const {
  assert(in0.getPosition() + 1 == maxBlockSize_);
  assert(in1.getPosition() + 1 == elementsCnt_ - maxBlockSize_);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::removeTmp_() {
  tapePool_->removeTape(tmpDirectoryName_ + "/tmp_tape_00");
  tapePool_->removeTape(tmpDirectoryName_ + "/tmp_tape_01");
  tapePool_->removeTape(tmpDirectoryName_ + "/tmp_tape_10");
  tapePool_->removeTape(tmpDirectoryName_ + "/tmp_tape_11");

  if (needToRemoveTmpDirectory_) {
    std::filesystem::remove(tmpDirectoryName_);
  }
}
