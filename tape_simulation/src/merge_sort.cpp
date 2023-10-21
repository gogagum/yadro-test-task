#include <array>
#include <cassert>
#include <merge_sort.hpp>
#include <tape_pool.hpp>

#include "merge.hpp"
#include "tape_view_read_iterators.hpp"
#include "tape_view_write_iterators.hpp"

////////////////////////////////////////////////////////////////////////////////
void MergeSort::perform(std::string_view outFilename) && {
  auto srcTape = tapePool_.getOpenedTapeView(inFilename_);

  const auto [iterationsCnt, maxBlockSize] =
      getIterationsCntAndMaxBlockSize_(1);

  const std::string tmpTapeName00 = tmpDirectory_ + "/tmpTape00";
  const std::string tmpTapeName01 = tmpDirectory_ + "/tmpTape01";
  const std::string tmpTapeName10 = tmpDirectory_ + "/tmpTape10";
  const std::string tmpTapeName11 = tmpDirectory_ + "/tmpTape11";

  auto tape00 = tapePool_.createTape(tmpTapeName00, maxBlockSize);
  auto tape01 =
      tapePool_.createTape(tmpTapeName01, elementsCnt_ - maxBlockSize);
  auto tape10 = tapePool_.createTape(tmpTapeName10, maxBlockSize);
  auto tape11 =
      tapePool_.createTape(tmpTapeName11, elementsCnt_ - maxBlockSize);

  {
    auto out0 = tapePool_.getOpenedTapeView(tmpTapeName00);
    auto out1 = tapePool_.getOpenedTapeView(tmpTapeName01);
    makeInitialBlocks_(srcTape, out0, out1);
  }

  std::size_t iteration = 0;
  std::size_t blockSize = 1;

  for (; iteration < iterationsCnt; ++iteration, blockSize *= 2) {
    if (iteration % 2 == 0) {
      mergeBlocksLeft_(tape00, tape01, tape10, tape11, blockSize);
      tape00.moveRight();
      tape01.moveRight();
      tape10.moveRight();
      tape10.moveRight();
    } else {
      mergeBlocksRight_(tape10, tape11, tape00, tape01, blockSize);
      tape00.moveLeft();
      tape01.moveLeft();
      tape10.moveLeft();
      tape10.moveLeft();
    }
  }

  blockSize /= 2;

  auto outTape = tapePool_.createTape(outFilename, elementsCnt_);

  if (iteration % 2 == 0) {  // resulting merge is left
    outTape.moveRight(elementsCnt_ - 1);

    merge_decreasing(LeftReadIterator(tape00), blockSize,
                     LeftReadIterator(tape01), elementsCnt_ - blockSize,
                     LeftWriteIterator(outTape));
  } else {
    merge_increasing(RightReadIterator(tape10), blockSize,
                     RightReadIterator(tape11), elementsCnt_ - blockSize,
                     RightWriteIterator(outTape));
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
  std::copy_n(read, to0Cnt, RightWriteIterator(out0));
  std::copy_n(read, to1Cnt, RightWriteIterator(out1));
}

////////////////////////////////////////////////////////////////////////////////
void MergeSort::mergeBlocksRight_(TapeView& in0, TapeView& in1, TapeView& out0,
                                  TapeView& out1, std::size_t blockSize) const {
  assert(in0.getPosition() == 0 &&
         "Right merge must start from zero position.");
  assert(in1.getPosition() == 0 &&
         "Right merge must start from zero position.");

  const auto inBlocksCnt = getBlocksCnt_(blockSize);
  const auto [inBlocksCnt0, inBlocksCnt1] = getBlocksCnts_(blockSize);

  const auto outBlocksSize = blockSize * 2;
  const auto outBlocksCnt = getBlocksCnt_(outBlocksSize);
  assert(outBlocksCnt >= 1 && "Must create at least one block.");
  const auto [outBlocksCnt0, outBlocksCnt1] = getBlocksCnts_(outBlocksSize);

  assert(inBlocksCnt1 >= outBlocksCnt && "Must have enough blocks to merge.");

  auto read0 = RightReadIterator(in0);
  auto read1 = RightReadIterator(in1);

  auto write0 = RightWriteIterator(out0);
  auto write1 = RightWriteIterator(out1);

  std::size_t mergedBlockPairIdx = 0;

  for (std::size_t i = 0; i < outBlocksCnt0; ++i, ++mergedBlockPairIdx) {
    merge_increasing(read0, blockSize, read1, blockSize, write0);
  }

  for (std::size_t i = 0; i < outBlocksCnt1; ++i, ++mergedBlockPairIdx) {
    merge_increasing(read0, blockSize, read1, blockSize, write1);
  }

  const auto elementsLeft = elementsCnt_ - mergedBlockPairIdx * blockSize;
  auto& tailWrite = (outBlocksCnt % 2 == 0) ? write0 : write1;

  if (inBlocksCnt % 2 == 0) {
    std::copy_n(read0, elementsLeft, tailWrite);
  } else {
    const auto elementsLeft0 = blockSize;
    const auto elementsLeft1 = elementsLeft - elementsLeft0;
    merge_increasing(read0, elementsLeft0, read1, elementsLeft1, tailWrite);
  }

  assert(in0.getPosition() + in1.getPosition() == elementsCnt_ &&
         "Must read all.");
  assert(out0.getPosition() + out1.getPosition() == elementsCnt_ &&
         "Must write all.");
}

////////////////////////////////////////////////////////////////////////////////
void MergeSort::mergeBlocksLeft_(TapeView& in0, TapeView& in1, TapeView& out0,
                                 TapeView& out1, std::size_t blockSize) const {
  const auto inBlocksCnt = getBlocksCnt_(blockSize);
  const auto [inBlocksCnt0, inBlocksCnt1] = getBlocksCnts_(blockSize);

  const auto inCnt0 = (inBlocksCnt % 2 == 0)
                          ? elementsCnt_ - inBlocksCnt1 * blockSize
                          : inBlocksCnt0 * blockSize;

  const auto inCnt1 = (inBlocksCnt % 2 == 0)
                          ? inBlocksCnt1 * blockSize
                          : elementsCnt_ - inBlocksCnt0 * blockSize;

  assert(in0.getPosition() == inCnt0 - 1 &&
         "in0 must be positioned on the last element after right merge.");

  assert(in1.getPosition() == inCnt1 - 1 &&
         "in1 must be positioned on the last element after right merge.");

  const auto outBlockSize = blockSize * 2;
  const auto outBlocksCnt = getBlocksCnt_(outBlockSize);
  const auto [outBlocksCnt0, outBlocksCnt1] = getBlocksCnts_(outBlockSize);

  const auto outElements0 = outBlockSize * outBlocksCnt0;
  const auto outElements1 = elementsCnt_ - outElements0;
  const auto neededPosition0 = outElements0 - 1;
  const auto neededPosition1 = outElements1 - 1;

  assert(out0.getPosition() <= neededPosition0);
  const auto toMove0 = neededPosition0 - out0.getPosition();
  assert(out1.getPosition() >= neededPosition1);
  const auto toMove1 = out1.getPosition() - neededPosition1;
  assert(toMove0 == toMove1);

  out0.moveRight(toMove0);
  out1.moveLeft(toMove1);

  auto read0 = LeftReadIterator(in0);
  auto read1 = LeftReadIterator(in1);

  auto write0 = LeftWriteIterator(out0);
  auto write1 = LeftWriteIterator(out1);

  auto& tailWrite = (outBlocksCnt % 2 == 0) ? write0 : write1;

  if (inBlocksCnt % 2 == 0) {
    const auto tailSize = inCnt0 - inBlocksCnt0 * blockSize;
    std::copy_n(read0, tailSize, tailWrite);
  } else {
    const auto tailSize = inCnt1 - inBlocksCnt1 * blockSize;
    merge_decreasing(read0, blockSize, read1, tailSize, tailWrite);
  }

  assert(in0.getPosition() == in1.getPosition() &&
         "In tapes must be in same positions.");

  for (std::size_t i = 0; i < outBlocksCnt0; ++i) {
    merge_decreasing(read0, blockSize, read1, blockSize, write0);
  }

  for (std::size_t i = 0; i < outBlocksCnt1; ++i) {
    merge_decreasing(read0, blockSize, read1, blockSize, write1);
  }

  assert(in0.getPosition() == -1 && "in0 must come to the beginning.");
  assert(in1.getPosition() == -1 && "in1 must come to the beginning.");
  assert(out0.getPosition() == -1 && "out0 must come to the beginning.");
  assert(out1.getPosition() == -1 && "out1 must come to the beginning.");
}
