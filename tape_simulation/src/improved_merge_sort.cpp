#include <array>
#include <cassert>
#include <filesystem>
#include <improved_merge_sort.hpp>
#include <tape_pool.hpp>

#include "copy_elements_sorted.hpp"
#include "tape_view_read_iterators.hpp"
#include "tape_view_write_iterators.hpp"

////////////////////////////////////////////////////////////////////////////////
ImprovedMergeSortImproved::ZeroHeapSizeLimit::ZeroHeapSizeLimit()
    : std::logic_error("heapSizeLimit can not be zero.") {
}

////////////////////////////////////////////////////////////////////////////////
ImprovedMergeSortImproved::ImprovedMergeSortImproved(
    TapePool& tapePool, std::string_view inFilename,
    std::string_view tmpDirectory, bool increasing, std::size_t heapSizeLimit)
    : MergeSortImpl(tapePool, inFilename, tmpDirectory, heapSizeLimit,
                    increasing) {
  if (heapSizeLimit == 0) {
    throw ZeroHeapSizeLimit();
  }
}

////////////////////////////////////////////////////////////////////////////////
void ImprovedMergeSortImproved::perform(std::string_view outFilename) && {
  auto inTape = tapePool_->getOrOpenTape(inFilename_);
  auto outTape = tapePool_->createTape(std::string(outFilename), elementsCnt_);

  if (inTape.getPosition() != 0) {
    throw std::logic_error("Input tape head is not in the beginning.");
  }

  if (elementsCnt_ == 0) {
    tapePool_->closeTape(std::string(outFilename));
    return;
  }

  if (elementsCnt_ == 1) {
    outTape.write(inTape.read());
    tapePool_->closeTape(std::string(outFilename));
    return;
  }

  if (elementsCnt_ <= initialBlockSize_) {
    auto read = RightReadIterator(inTape);
    auto write = RightWriteIterator(outTape);
    if (increasing_) {
      copy_top_elements_sorted(read, write, elementsCnt_);
    } else {
      copy_bottom_elements_sorted(read, write, elementsCnt_);
    }
    tapePool_->closeTape(std::string(outFilename));
    return;
  }

  makeInitialBlocks_(inTape, tapesManager_.getInitialOutTape0(),
                     tapesManager_.getInitialOutTape1());

  std::size_t iterationsLeft = iterationsCnt_;
  std::size_t blockSize = initialBlockSize_;

  for (; iterationsLeft > 0; --iterationsLeft, blockSize *= 2) {
    const std::size_t iterationIdx = iterationsCnt_ - iterationsLeft;
    mergeBlocks_(blockSize, iterationIdx, iterationsLeft);
  }

  mergeIntoOutputTape_(tapesManager_.getInTape0(iterationsCnt_),
                       tapesManager_.getInTape1(iterationsCnt_), outTape);
  tapePool_->closeTape(std::string(outFilename));
}

////////////////////////////////////////////////////////////////////////////////
void ImprovedMergeSortImproved::makeInitialBlocks_(TapeView& in, TapeView& out0,
                                                   TapeView& out1) const {
  const auto [blocksOut0, blocksOut1] = getBlocksCnts_(initialBlockSize_);
  auto read = RightReadIterator(in);
  auto write0 = RightWriteIterator(out0);
  auto write1 = RightWriteIterator(out1);
  bool increasing = (iterationsCnt_ % 2 == 0) ? !increasing_ : increasing_;
  for (std::size_t i = 0; i < blocksOut0; ++i) {
    copyElementsSorted_(read, write0, initialBlockSize_, increasing);
    if (i + 1 != blocksOut0) {
      ++read;
      ++write0;
    }
  }

  if (blocksOut1 != 0) {
    ++read;
    for (std::size_t i = 0; i < blocksOut1; ++i) {
      copyElementsSorted_(read, write1, initialBlockSize_, increasing);
      if (i + 1 != blocksOut1) {
        ++read;
        ++write1;
      }
    }
  }

  if (std::size_t tailSize = elementsCnt_ % initialBlockSize_; tailSize != 0) {
    auto write = (blocksOut0 == blocksOut1 + 1) ? write1 : write0;
    if (const auto blockBeforeTail =
            (blocksOut0 == blocksOut1 + 1) ? blocksOut1 : blocksOut0;
        blockBeforeTail != 0) {
      ++write;
    }
    ++read;
    copyElementsSorted_(read, write, tailSize, increasing);
  }
}

////////////////////////////////////////////////////////////////////////////////
void ImprovedMergeSortImproved::copyElementsSorted_(RightReadIterator read,
                                                    RightWriteIterator write,
                                                    std::size_t cnt,
                                                    bool increasing) {
  if (increasing) {
    copy_top_elements_sorted(read, write, cnt);
  } else {
    copy_bottom_elements_sorted(read, write, cnt);
  }
}
