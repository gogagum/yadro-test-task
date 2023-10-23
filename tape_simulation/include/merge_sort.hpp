#ifndef TAPE_SIMULATION_MERGE_SORT_HPP
#define TAPE_SIMULATION_MERGE_SORT_HPP

#include <cassert>
#include <string>
#include <string_view>

#include "copy_n.hpp"
#include "merge.hpp"
#include "tape_pool.hpp"
#include "tape_view.hpp"
#include "tape_view_read_iterators.hpp"
#include "tape_view_write_iterators.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief class MergeSort - merge sort class.
class MergeSort {
 public:
  MergeSort(TapePool& tapePool, std::string_view inFilename,
            std::string_view tmpDirectory);

  void perform(std::string_view outFilename) &&;

 private:
  struct OperationBlocksCnts_ {
    std::size_t blocksIn;
    std::size_t blocksIn0;
    std::size_t blocksIn1;
    std::size_t blocksOut;
    std::size_t blocksOut0;
    std::size_t blocksOut1;
  };

  struct Counts_ {
    std::size_t cnt0;
    std::size_t cnt1;
  };

  enum Order_ { increasing, decreasing };

 private:
  static std::size_t getBlockSize_(std::size_t iteration);

  [[nodiscard]] std::pair<std::size_t, std::size_t>
  getIterationsCntAndMaxBlockSize_(std::size_t startBlockSize) const;

  void makeInitialBlocks_(TapeView& in, TapeView& out0, TapeView& out1) const;

  /**
   * @brief mergeBlocksForward_
   * in:
   * Partial couple is at the end of in0 and in1 and is read first.
   * Blocks are sorted decreasing and are read increasing.
   * out:
   * Partial couple is in the beginnings of out0 and out1 and written first.
   * Blocks are written increasing and then can be read decreasing.
   * This type of merge must happen on last operation between couples.
   * (iterationsLeft = 2, 4, 6...)
   * Note that first iteration can be made both with mergeBlocksForward_ and
   * mergeBlocksBackward_ as in the beginning blocks are of size 1 and are both
   * sorted increasing and decreasing.
   *
   * @param in0 first source tape. The head must be at the end of last block or
   * at the and of a part of last block.
   * @param in1 second source tape. The head must be at the end of last block or
   * at the and of a part of last block.
   * @param out0 first output tape. The head must be in the beginning of a tape.
   * @param out1 second output tape. The head must be in the beginning of a
   * tape.
   * @param blockSize block size.
   */
  void mergeBlocks0_(TapeView& in0, TapeView& in1, TapeView& out0,
                     TapeView& out1, std::size_t blockSize) const;

  /**
   * @brief mergeBlocksBackward_
   *
   * in:
   * Partial couple is in the beginnings of in1 and in2 and is read last.
   * Blocks are sorted increasing and are read decreasing.
   * out:
   * Partial couple is in the end of out0 and out1 and written last.
   * Blocks are written decreasing and then can be read decreasing.
   * This type of merge must happen on last operation between couples
   * (iterationsLeft = 1, 3, 5...).
   *
   * @param in0 first source tape. The head must be at the end of last block
   * full block.
   * @param in1 second source tape. The head must be at the end of last full
   * block.
   * @param out0 first output tape. The head must be in the beginning of a tape.
   * @param out1 second output tape. The head must be in the beginning of a
   * tape.
   * @param blockSize block size.
   */
  void mergeBlocks1_(TapeView& in0, TapeView& in1, TapeView& out0,
                     TapeView& out1, std::size_t blockSize) const;

  [[nodiscard]] OperationBlocksCnts_ calcOperationBlocksCnts_(
      std::size_t blockSize) const;

  [[nodiscard]] Counts_ calcCounts_(std::size_t blocksCnt0,
                                    std::size_t blocksCnt1,
                                    std::size_t blockSize) const;

  [[nodiscard]] Counts_ calcTailsCounts_(std::size_t elementsCnt0,
                                         std::size_t elementsCnt1,
                                         std::size_t blocksCnt1,
                                         std::size_t blockSize) const;

  [[nodiscard]] std::size_t getBlocksCnt_(std::size_t blockSize) const;

  [[nodiscard]] std::size_t getBlocksCnt0_(std::size_t blockSize) const;

  [[nodiscard]] std::size_t getBlocksCnt1_(std::size_t blockSize) const;

  [[nodiscard]] Counts_ getBlocksCnts_(std::size_t blockSize) const;

  template <Order_ order, bool moveInTheEnd>
  void processPartialBlocks_(LeftReadIterator& in0, std::size_t cnt0,
                             LeftReadIterator& in1, std::size_t cnt1,
                             RightWriteIterator& out0) const;

  template <Order_ order>
  void processBlocksPairs_(LeftReadIterator& in0, LeftReadIterator& in1,
                           RightWriteIterator& out0, std::size_t blocksOut0,
                           RightWriteIterator& out1, std::size_t blocksOUt1,
                           std::size_t blockSize) const;

  void checkStartPositions_(TapeView& in0, TapeView& in1, TapeView& out0,
                            TapeView& out1, std::size_t inCnt0Expected,
                            std::size_t inCnt1Expected) const;

  void checkFinishPositions_(TapeView& in0, TapeView& in1, TapeView& out0,
                             TapeView& out1, std::size_t outCnt0Expected,
                             std::size_t outCnt1Expected) const;

 private:
  TapePool* tapePool_;
  std::size_t elementsCnt_;
  std::string inFilename_;
  std::string tmpDirectory_;
  std::size_t firstHalfSize_;
  std::size_t secondHalfSize_;
  bool needToRemoveTmpPath_{false};
};

////////////////////////////////////////////////////////////////////////////////
inline std::size_t getBlockSize_(std::size_t iteration) {
  return std::size_t{1} << (iteration + 1);
}

////////////////////////////////////////////////////////////////////////////////
inline auto MergeSort::calcTailsCounts_(std::size_t elementsCnt0,
                                        std::size_t elementsCnt1,
                                        std::size_t blocksCnt1,
                                        std::size_t blockSize) const
    -> Counts_ {
  return {elementsCnt0 - blocksCnt1 * blockSize,
          elementsCnt1 - blocksCnt1 * blockSize};
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t MergeSort::getBlocksCnt_(std::size_t blockSize) const {
  return elementsCnt_ / blockSize;
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t MergeSort::getBlocksCnt0_(std::size_t blockSize) const {
  return (getBlocksCnt_(blockSize) - 1) / 2 + 1;
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t MergeSort::getBlocksCnt1_(std::size_t blockSize) const {
  return getBlocksCnt_(blockSize) / 2;
}

////////////////////////////////////////////////////////////////////////////////
inline auto MergeSort::getBlocksCnts_(std::size_t blockSize) const -> Counts_ {
  const auto blocksCnt = getBlocksCnt_(blockSize);
  return {(blocksCnt - 1) / 2 + 1, blocksCnt / 2};
}

////////////////////////////////////////////////////////////////////////////////
template <MergeSort::Order_ order, bool moveInTheEnd>
inline void MergeSort::processPartialBlocks_(LeftReadIterator& in0,
                                             std::size_t cnt0,
                                             LeftReadIterator& in1,
                                             std::size_t cnt1,
                                             RightWriteIterator& out0) const {
  if (cnt1 == 0) {
    copy_n(in0, cnt0, out0);
    if constexpr (moveInTheEnd) {
      ++in0;
      ++out0;
    }
  } else {
    if constexpr (order == Order_::increasing) {
      merge_increasing(in0, cnt0, in1, cnt1, out0);
    } else {
      merge_decreasing(in0, cnt0, in1, cnt1, out0);
    }
    if constexpr (moveInTheEnd) {
      ++in0;
      ++in1;
      ++out0;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
template <MergeSort::Order_ order>
void MergeSort::processBlocksPairs_(
    LeftReadIterator& in0, LeftReadIterator& in1, RightWriteIterator& out0,
    std::size_t blocksOut0, RightWriteIterator& out1, std::size_t blocksOut1,
    std::size_t blockSize) const {
  assert(blocksOut0 >= blocksOut1);
  assert(blocksOut1 + 1 >= blocksOut0);

  for (std::size_t i = 0; i < blocksOut0; ++i) {
    if constexpr (order == increasing) {
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
      if constexpr (order == increasing) {
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

#endif  // TAPE_SIMULATION_MERGE_SORT_HPP
