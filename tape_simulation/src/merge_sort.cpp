#include <array>
#include <cassert>
#include <filesystem>
#include <merge_sort.hpp>
#include <tape_pool.hpp>

#include "merge.hpp"
#include "tape_view_read_iterators.hpp"
#include "tape_view_write_iterators.hpp"

////////////////////////////////////////////////////////////////////////////////
MergeSort::MergeSort(TapePool& tapePool, std::string_view inFilename,
                     std::string_view tmpDirectory)
    : tapePool_{&tapePool},
      elementsCnt_{tapePool_->getOrOpenTape(inFilename).getSize()},
      inFilename_{inFilename},
      tmpDirectory_{tmpDirectory} {
  if (!std::filesystem::exists(tmpDirectory)) {
    std::filesystem::create_directory(tmpDirectory);
    needToRemoveTmpPath_ = true;
  }
}

////////////////////////////////////////////////////////////////////////////////
void MergeSort::perform(std::string_view outFilename) && {
  auto inTape = tapePool_->getOpenedTape(inFilename_);
  auto outTape = tapePool_->createTape(outFilename, elementsCnt_);

  if (elementsCnt_ == 0) {
    return;
  }

  if (elementsCnt_ == 1) {
    outTape.write(inTape.read());
    outTape.moveRight();
    inTape.moveRight();
    return;
  }

  const auto [iterationsCnt, maxBlockSize] =
      getIterationsCntAndMaxBlockSize_(1);

  const std::string tmpTapeName00 = tmpDirectory_ + "/tmpTape00";
  const std::string tmpTapeName01 = tmpDirectory_ + "/tmpTape01";
  const std::string tmpTapeName10 = tmpDirectory_ + "/tmpTape10";
  const std::string tmpTapeName11 = tmpDirectory_ + "/tmpTape11";

  auto tape00 = tapePool_->createTape(tmpTapeName00, maxBlockSize);
  auto tape01 = tapePool_->createTape(tmpTapeName01, maxBlockSize);
  auto tape10 = tapePool_->createTape(tmpTapeName10, maxBlockSize);
  auto tape11 = tapePool_->createTape(tmpTapeName11, maxBlockSize);

  {
    auto out0 = tapePool_->getOpenedTape(tmpTapeName00);
    auto out1 = tapePool_->getOpenedTape(tmpTapeName01);
    makeInitialBlocks_(inTape, out0, out1);
  }

  std::size_t iterationsLeft = iterationsCnt;
  std::size_t blockSize = 1;

  for (; iterationsLeft > 0; --iterationsLeft, blockSize *= 2) {
    std::size_t iterationIdx = iterationsCnt - iterationsLeft;
    auto& in0 = (iterationIdx % 2 == 0) ? tape00 : tape10;
    auto& in1 = (iterationIdx % 2 == 0) ? tape01 : tape11;
    auto& out0 = (iterationIdx % 2 == 0) ? tape10 : tape00;
    auto& out1 = (iterationIdx % 2 == 0) ? tape11 : tape01;

    if (iterationsLeft % 2 == 1) {
      mergeBlocks1_(in0, in1, out0, out1, blockSize);
    } else {
      mergeBlocks0_(in0, in1, out0, out1, blockSize);
    }
  }

  auto& inTape0 = (iterationsCnt % 2 == 0) ? tape00 : tape10;
  auto& inTape1 = (iterationsCnt % 2 == 0) ? tape01 : tape11;

  assert(inTape0.getPosition() + 1 == maxBlockSize &&
         "First final tape must contain full max block.");
  assert(inTape1.getPosition() + 1 == elementsCnt_ - maxBlockSize &&
         "Second final tape must contain rest elements.");

  auto in0 = LeftReadIterator(inTape0);
  auto in1 = LeftReadIterator(inTape1);
  auto out = RightWriteIterator(outTape);

  merge_increasing(in0, maxBlockSize, in1, elementsCnt_ - maxBlockSize, out);

  tapePool_->removeTape(tmpTapeName00);
  tapePool_->removeTape(tmpTapeName01);
  tapePool_->removeTape(tmpTapeName10);
  tapePool_->removeTape(tmpTapeName11);

  if (needToRemoveTmpPath_) {
    std::filesystem::remove(tmpDirectory_);
  }
}

////////////////////////////////////////////////////////////////////////////////
std::pair<std::size_t, std::size_t> MergeSort::getIterationsCntAndMaxBlockSize_(
    std::size_t startBlockSize) const {
  assert(startBlockSize < elementsCnt_ &&
         "Start block size must be smaller than sorted tape.");

  std::size_t iterationsCnt = 0;
  std::size_t blockSize = startBlockSize;
  for (; blockSize * 2 < elementsCnt_; blockSize *= 2, ++iterationsCnt) {
  }

  return std::make_pair(iterationsCnt, blockSize);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSort::makeInitialBlocks_(TapeView& in, TapeView& out0,
                                   TapeView& out1) const {
  const auto [to0Cnt, to1Cnt] = getBlocksCnts_(1);
  auto read = RightReadIterator(in);
  copy_n(read, to0Cnt, RightWriteIterator(out0));
  ++read;
  copy_n(read, to1Cnt, RightWriteIterator(out1));
}

////////////////////////////////////////////////////////////////////////////////
void MergeSort::mergeBlocks0_(TapeView& in0, TapeView& in1, TapeView& out0,
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
    processPartialBlocks_<Order_::increasing, true>(read0, inTailSize0, read1,
                                                    inTailSize1, tailWrite);
  }

  processBlocksPairs_<Order_::increasing>(read0, read1, write0,
                                          opBlocksCnt.blocksOut0, write1,
                                          opBlocksCnt.blocksOut1, blockSize);

  const auto [outCnt0, outCnt1] = calcCounts_(
      opBlocksCnt.blocksOut0, opBlocksCnt.blocksOut1, blockSize * 2);

  checkFinishPositions_(in0, in1, out0, out1, outCnt0, outCnt1);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSort::mergeBlocks1_(TapeView& in0, TapeView& in1, TapeView& out0,
                              TapeView& out1, std::size_t blockSize) const {
  const auto opBlocksCnt = calcOperationBlocksCnts_(blockSize);

  const auto [inCnt0, inCnt1] =
      calcCounts_(opBlocksCnt.blocksIn0, opBlocksCnt.blocksIn1, blockSize);

  checkStartPositions_(in0, in1, out0, out1, inCnt0, inCnt1);

  auto read0 = LeftReadIterator(in0);
  auto read1 = LeftReadIterator(in1);

  auto write0 = RightWriteIterator(out0);
  auto write1 = RightWriteIterator(out1);

  processBlocksPairs_<Order_::decreasing>(read0, read1, write0,
                                          opBlocksCnt.blocksOut0, write1,
                                          opBlocksCnt.blocksOut1, blockSize);

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
    processPartialBlocks_<Order_::decreasing, false>(read0, inTailSize0, read1,
                                                     inTailSize1, tailWrite);
  }

  const auto [outCnt0, outCnt1] = calcCounts_(
      opBlocksCnt.blocksOut0, opBlocksCnt.blocksOut1, blockSize * 2);

  checkFinishPositions_(in0, in1, out0, out1, outCnt0, outCnt1);
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSort::calcOperationBlocksCnts_(std::size_t blockSize) const
    -> OperationBlocksCnts_ {
  auto inBlocksCnt = getBlocksCnt_(blockSize);
  auto [inBlocksCnt0, inBlocksCnt1] = getBlocksCnts_(blockSize);
  auto outBlockCnt = getBlocksCnt_(blockSize * 2);
  auto [outBlockCnt0, outBlockCnt1] = getBlocksCnts_(blockSize * 2);
  return {inBlocksCnt, inBlocksCnt0, inBlocksCnt1,
          outBlockCnt, outBlockCnt0, outBlockCnt1};
}

////////////////////////////////////////////////////////////////////////////////
auto MergeSort::calcCounts_(std::size_t blocksCnt0, std::size_t blocksCnt1,
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
void MergeSort::checkStartPositions_(TapeView& in0, TapeView& in1,
                                     TapeView& out0, TapeView& out1,
                                     std::size_t inCnt0Expected,
                                     std::size_t inCnt1Expected) const {
  assert(in0.getPosition() + 1 == inCnt0Expected);
  assert(in1.getPosition() + 1 == inCnt1Expected);
  assert(out0.getPosition() == 0);
  assert(out1.getPosition() == 0);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSort::checkFinishPositions_(TapeView& in0, TapeView& in1,
                                      TapeView& out0, TapeView& out1,
                                      std::size_t outCnt0Expected,
                                      std::size_t outCnt1Expected) const {
  assert(in0.getPosition() == 0);
  assert(in1.getPosition() == 0);
  assert(out0.getPosition() + 1 == outCnt0Expected);
  assert(out1.getPosition() + 1 == outCnt1Expected);
}
