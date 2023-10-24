#ifndef TAPE_SIMULATION_IMPL_MERGE_SORT_HPP
#define TAPE_SIMULATION_IMPL_MERGE_SORT_HPP

#include <cstdint>
#include <string>

#include "../tape_pool.hpp"
#include "../tape_view_read_iterators.hpp"
#include "../tape_view_write_iterators.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief class MergeSortImpl
class MergeSortImpl {
 protected:
  MergeSortImpl(TapePool& tapePool, std::string_view inFilename,
                std::string_view tmpDirectory);

 protected:
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

 protected:
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

  [[nodiscard]] TapePool& getTapePool_() const;

  [[nodiscard]] std::size_t getElementsCnt_() const;

  [[nodiscard]] const std::string& getInFilename_() const;

  [[nodiscard]] const std::string& getTmpDirectoryName_() const;

  [[nodiscard]] bool needToRemoveTmpDirectory_() const;

  [[nodiscard]] Counts_ getBlocksCnts_(std::size_t blockSize) const;

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

  void checkStartPositions_(TapeView& in0, TapeView& in1, TapeView& out0,
                            TapeView& out1, std::size_t inCnt0Expected,
                            std::size_t inCnt1Expected) const;

  void checkFinishPositions_(TapeView& in0, TapeView& in1, TapeView& out0,
                             TapeView& out1, std::size_t outCnt0Expected,
                             std::size_t outCnt1Expected) const;

  [[nodiscard]] OperationBlocksCnts_ calcOperationBlocksCnts_(
      std::size_t blockSize) const;

  void processPartialBlocks_(LeftReadIterator& in0, std::size_t cnt0,
                             LeftReadIterator& in1, std::size_t cnt1,
                             RightWriteIterator& out0, bool increasing,
                             bool moveInTheEnd) const;

  void processBlocksPairs_(LeftReadIterator& in0, LeftReadIterator& in1,
                           RightWriteIterator& out0, std::size_t blocksOut0,
                           RightWriteIterator& out1, std::size_t blocksOUt1,
                           std::size_t blockSize, bool increasing) const;

  static std::size_t getBlockSize_(std::size_t iteration);

  [[nodiscard]] std::pair<std::size_t, std::size_t>
  getIterationsCntAndMaxBlockSize_(std::size_t startBlockSize) const;

  void makeInitialBlocks_(TapeView& in, TapeView& out0, TapeView& out1) const;

 private:
  TapePool* tapePool_;
  std::size_t elementsCnt_;
  std::string inFilename_;
  std::string tmpDirectoryName_;
  std::size_t firstHalfSize_;
  std::size_t secondHalfSize_;
  bool needToRemoveTmpDirectoryFlag_{false};
};

////////////////////////////////////////////////////////////////////////////////
inline TapePool& MergeSortImpl::getTapePool_() const {
  return *tapePool_;
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t MergeSortImpl::getElementsCnt_() const {
  return elementsCnt_;
}

////////////////////////////////////////////////////////////////////////////////
inline const std::string& MergeSortImpl::getInFilename_() const {
  return inFilename_;
}

////////////////////////////////////////////////////////////////////////////////
inline const std::string& MergeSortImpl::getTmpDirectoryName_() const {
  return tmpDirectoryName_;
}

////////////////////////////////////////////////////////////////////////////////
inline bool MergeSortImpl::needToRemoveTmpDirectory_() const {
  return needToRemoveTmpDirectoryFlag_;
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t MergeSortImpl::getBlocksCnt_(std::size_t blockSize) const {
  return elementsCnt_ / blockSize;
}

////////////////////////////////////////////////////////////////////////////////
inline auto MergeSortImpl::calcTailsCounts_(std::size_t elementsCnt0,
                                            std::size_t elementsCnt1,
                                            std::size_t blocksCnt1,
                                            std::size_t blockSize) const
    -> Counts_ {
  return {elementsCnt0 - blocksCnt1 * blockSize,
          elementsCnt1 - blocksCnt1 * blockSize};
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t MergeSortImpl::getBlocksCnt0_(std::size_t blockSize) const {
  return (getBlocksCnt_(blockSize) - 1) / 2 + 1;
}

////////////////////////////////////////////////////////////////////////////////
inline std::size_t MergeSortImpl::getBlocksCnt1_(std::size_t blockSize) const {
  return getBlocksCnt_(blockSize) / 2;
}

////////////////////////////////////////////////////////////////////////////////
inline auto MergeSortImpl::getBlocksCnts_(std::size_t blockSize) const
    -> Counts_ {
  const auto blocksCnt = getBlocksCnt_(blockSize);
  return {(blocksCnt - 1) / 2 + 1, blocksCnt / 2};
}

#endif
