#include <copy_n.hpp>
#include <impl/merge_sort.hpp>
#include <merge.hpp>

////////////////////////////////////////////////////////////////////////////////
MergeSortImpl::MergeSortImpl(TapePool& tapePool, std::string_view inFilename,
                             std::string_view tmpDirectory,
                             std::size_t initialBlockSize, bool increasing)
    : MergeSortArithmeticsBase(
          tapePool.getOrOpenTape(std::string(inFilename)).getSize(),
          initialBlockSize),
      tapePool_{&tapePool},
      inFilename_{inFilename},
      increasing_{increasing},
      tapesManager_(tapePool, tmpDirectory, maxBlockSize_) {
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::processPartialBlocks_(
    LeftReadIterator& in0, std::size_t cnt0, LeftReadIterator& in1,
    std::size_t cnt1, RightWriteIterator& out, bool increasing) const {
  if (cnt1 == 0) {
    copy_n(in0, cnt0, out);
  } else {
    merge_(in0, cnt0, in1, cnt1, out, increasing);
  }
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::processBlocksPairs_(
    LeftReadIterator& in0, LeftReadIterator& in1, RightWriteIterator& out0,
    std::size_t blocksOut0, RightWriteIterator& out1, std::size_t blocksOut1,
    std::size_t blockSize, bool increasing) const {
  for (std::size_t i = 0; i < blocksOut0; ++i) {
    merge_(in0, blockSize, in1, blockSize, out0, increasing);
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
      merge_(in0, blockSize, in1, blockSize, out1, increasing);
      if (i + 1 != blocksOut1) {
        ++in0;
        ++in1;
        ++out1;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks_(std::size_t blockSize,
                                 std::size_t iterationIndex,
                                 std::size_t iterationsLeft) {
  const bool increasing =
      (iterationsLeft % 2 == 1) ? !increasing_ : increasing_;
  auto& in0 = tapesManager_.getInTape0(iterationIndex);
  auto& in1 = tapesManager_.getInTape1(iterationIndex);
  auto& out0 = tapesManager_.getOutTape0(iterationIndex);
  auto& out1 = tapesManager_.getOutTape1(iterationIndex);
  if (iterationIndex % 2 == 1) {
    mergeBlocks1AndCheck_(in0, in1, out0, out1, blockSize, increasing);
  } else {
    mergeBlocks0AndCheck_(in0, in1, out0, out1, blockSize, increasing);
  }
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks0AndCheck_(TapeView& in0, TapeView& in1,
                                          TapeView& out0, TapeView& out1,
                                          std::size_t blockSize,
                                          bool increasing) const {
  checkStartPositions_(in0, in1, out0, out1, blockSize);

  mergeBlocks0_(LeftReadIterator(in0), LeftReadIterator(in1),
                RightWriteIterator(out0), RightWriteIterator(out1), blockSize,
                increasing);

  checkFinishPositions_(in0, in1, out0, out1, blockSize);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks0_(
    LeftReadIterator read0, LeftReadIterator read1, RightWriteIterator write0,
    RightWriteIterator write1, std::size_t blockSize, bool increasing) const {
  const auto [_0, _1, blocksIn1, blocksOut, blocksOut0, blocksOut1] =
      calcOperationBlocksCnts_(blockSize);

  auto [inTailSize0, inTailSize1] = calcTailsCounts_(blocksIn1, blockSize);

  if (inTailSize0 + inTailSize1 != 0) {
    const bool tailTo0 = (blocksOut % 2 == 0);
    auto& tailWrite = tailTo0 ? write0 : write1;

    processPartialBlocks_(read0, inTailSize0, read1, inTailSize1, tailWrite,
                          increasing);
    if (const auto blocksAfterTailWrite = tailTo0 ? blocksOut0 : blocksOut1;
        blocksAfterTailWrite != 0) {
      ++tailWrite;
    }
    ++read0;
    if (inTailSize1 != 0) {
      ++read1;
    }
  }

  processBlocksPairs_(read0, read1, write0, blocksOut0, write1, blocksOut1,
                      blockSize, increasing);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks1AndCheck_(TapeView& in0, TapeView& in1,
                                          TapeView& out0, TapeView& out1,
                                          std::size_t blockSize,
                                          bool increasing) const {
  checkStartPositions_(in0, in1, out0, out1, blockSize);

  mergeBlocks1_(LeftReadIterator(in0), LeftReadIterator(in1),
                RightWriteIterator(out0), RightWriteIterator(out1), blockSize,
                increasing);

  checkFinishPositions_(in0, in1, out0, out1, blockSize);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeBlocks1_(
    LeftReadIterator read0, LeftReadIterator read1, RightWriteIterator write0,
    RightWriteIterator write1, std::size_t blockSize, bool increasing) const {
  const auto [_0, _1, blocksIn1, blocksOut, blocksOut0, blocksOut1] =
      calcOperationBlocksCnts_(blockSize);

  processBlocksPairs_(read0, read1, write0, blocksOut0, write1, blocksOut1,
                      blockSize, increasing);

  auto [inTailSize0, inTailSize1] = calcTailsCounts_(blocksIn1, blockSize);

  if (inTailSize0 != 0 || inTailSize1 != 0) {
    auto& tailWrite = (blocksOut % 2 == 0) ? write0 : write1;

    if (blocksOut != 0) {
      ++read0;

      if (inTailSize1 != 0) {
        ++read1;
      }

      if (inTailSize0 + inTailSize1 != 0 && blocksOut1 != 0) {
        ++tailWrite;
      }
    }

    processPartialBlocks_(read0, inTailSize0, read1, inTailSize1, tailWrite,
                          increasing);
  }
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::mergeIntoOutputTape_(TapeView& inTape0, TapeView& inTape1,
                                         TapeView& outTape) const {
  checkFinalPositions_(inTape0, inTape1);

  merge_(LeftReadIterator(inTape0), maxBlockSize_, LeftReadIterator(inTape1),
         elementsCnt_ - maxBlockSize_, RightWriteIterator(outTape),
         increasing_);
}

////////////////////////////////////////////////////////////////////////////////
void MergeSortImpl::merge_(LeftReadIterator in0, std::size_t n0,
                           LeftReadIterator in1, std::size_t n1,
                           RightWriteIterator out, bool increasing) const {
  if (increasing) {
    merge_increasing(in0, n0, in1, n1, out);
  } else {
    merge_decreasing(in0, n0, in1, n1, out);
  }
}
