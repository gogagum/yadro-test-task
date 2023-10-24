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
    : MergeSortImpl(tapePool, inFilename, tmpDirectory, 1) {
}

////////////////////////////////////////////////////////////////////////////////
void MergeSort::perform(std::string_view outFilename) && {
  auto inTape = tapePool_->getOpenedTape(inFilename_);
  auto outTape = tapePool_->createTape(std::string(outFilename), elementsCnt_);

  if (elementsCnt_ == 0) {
    return;
  }

  if (elementsCnt_ == 1) {
    outTape.write(inTape.read());
    return;
  }

  makeInitialBlocks_(inTape, tmpTape00_, tmpTape01_);

  std::size_t iterationsLeft = iterationsCnt_;
  std::size_t blockSize = 1;

  for (; iterationsLeft > 0; --iterationsLeft, blockSize *= 2) {
    std::size_t iterationIdx = iterationsCnt_ - iterationsLeft;
    auto& in0 = (iterationIdx % 2 == 0) ? tmpTape00_ : tmpTape10_;
    auto& in1 = (iterationIdx % 2 == 0) ? tmpTape01_ : tmpTape11_;
    auto& out0 = (iterationIdx % 2 == 0) ? tmpTape10_ : tmpTape00_;
    auto& out1 = (iterationIdx % 2 == 0) ? tmpTape11_ : tmpTape01_;

    if (iterationsLeft % 2 == 1) {
      mergeBlocks1_(in0, in1, out0, out1, blockSize);
    } else {
      mergeBlocks0_(in0, in1, out0, out1, blockSize);
    }
  }

  auto& inTape0 = (iterationsCnt_ % 2 == 0) ? tmpTape00_ : tmpTape10_;
  auto& inTape1 = (iterationsCnt_ % 2 == 0) ? tmpTape01_ : tmpTape11_;

  checkFinalPositions_(inTape0, inTape1);

  auto in0 = LeftReadIterator(inTape0);
  auto in1 = LeftReadIterator(inTape1);
  auto out = RightWriteIterator(outTape);

  merge_increasing(in0, maxBlockSize_, in1, elementsCnt_ - maxBlockSize_, out);

  removeTmp_();
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

