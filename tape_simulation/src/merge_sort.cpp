#include <array>
#include <cassert>
#include <filesystem>
#include <merge_sort.hpp>
#include <tape_pool.hpp>

#include "copy_n.hpp"
#include "tape_view_read_iterators.hpp"
#include "tape_view_write_iterators.hpp"

////////////////////////////////////////////////////////////////////////////////
MergeSort::MergeSort(TapePool& tapePool, std::string_view inFilename,
                     std::string_view tmpDirectory, bool increasing)
    : MergeSortImpl(tapePool, inFilename, tmpDirectory, 1, increasing) {
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

  makeInitialBlocks_(inTape, tapesManager_.getInitialOutTape0(),
                     tapesManager_.getInitialOutTape1());

  std::size_t iterationsLeft = iterationsCnt_;
  std::size_t blockSize = 1;

  for (; iterationsLeft > 0; --iterationsLeft, blockSize *= 2) {
    const std::size_t iterationIdx = iterationsCnt_ - iterationsLeft;
    mergeBlocks_(blockSize, iterationIdx, iterationsLeft);
  }

  mergeIntoOutputTape_(tapesManager_.getInTape0(iterationsCnt_),
                       tapesManager_.getInTape1(iterationsCnt_), outTape);
  tapePool_->closeTape(std::string(outFilename));
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
