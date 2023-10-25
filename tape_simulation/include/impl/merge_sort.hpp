#ifndef TAPE_SIMULATION_IMPL_MERGE_SORT_HPP
#define TAPE_SIMULATION_IMPL_MERGE_SORT_HPP

#include <cstdint>
#include <string>

#include "../tape_pool.hpp"
#include "../tape_view_read_iterators.hpp"
#include "../tape_view_write_iterators.hpp"
#include "merge_sort_additional_tapes_manager.hpp"
#include "merge_sort_counter.hpp"
#include "tape_view.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief class MergeSortImpl
class MergeSortImpl : protected MergeSortArithmeticsBase {
 protected:
  MergeSortImpl(TapePool& tapePool, std::string_view inFilename,
                std::string_view tmpDirectory, std::size_t initialBlockSize,
                bool increasing);

 protected:
  /**
   * @brief mergeBlocks_ performs one of merge blocks depending on iteration
   * index.
   *
   * @param in0 first source tape. The head must be at the end of last block or
   * at the and of a part of last block.
   * @param in1 second source tape. The head must be at the end of last block or
   * at the and of a part of last block.
   * @param out0 first output tape. The head must be in the beginning of a tape.
   * @param out1 second output tape. The head must be in the beginning of a
   * tape.
   * @param blockSize block size.
   * @param iterationLeft iteration index from the end.
   */
  void mergeBlocks_(std::size_t blockSize, std::size_t iterationIndex,
                    std::size_t iterationsLeft);

  /**
   * @brief mergeBlocks0_
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
  void mergeBlocks0AndCheck_(TapeView& in0, TapeView& in1, TapeView& out0,
                     TapeView& out1, std::size_t blockSize,
                     bool increasing) const;

  void mergeBlocks0_(LeftReadIterator read0, LeftReadIterator read1,
                     RightWriteIterator write0, RightWriteIterator write1,
                     std::size_t blockSize, bool increasing) const;

  /**
   * @brief mergeBlocks1_
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
  void mergeBlocks1AndCheck_(TapeView& in0, TapeView& in1, TapeView& out0,
                     TapeView& out1, std::size_t blockSize,
                     bool increasing) const;

  void mergeBlocks1_(LeftReadIterator read0, LeftReadIterator read1,
                     RightWriteIterator write0, RightWriteIterator write1,
                     std::size_t blockSize, bool increasing) const;

  void processPartialBlocks_(LeftReadIterator& in0, std::size_t cnt0,
                             LeftReadIterator& in1, std::size_t cnt1,
                             RightWriteIterator& out0, bool increasing) const;

  void processBlocksPairs_(LeftReadIterator& in0, LeftReadIterator& in1,
                           RightWriteIterator& out0, std::size_t blocksOut0,
                           RightWriteIterator& out1, std::size_t blocksOUt1,
                           std::size_t blockSize, bool increasing) const;

  void mergeIntoOutputTape_(TapeView& inTape0, TapeView& inTape1,
                            TapeView& outTape) const;

 private:
  //////////////////////////////////////////////////////////////////////////////
  // Checks                                                                   //
  void checkStartPositions_(
      TapeView& in0, TapeView& in1, TapeView& out0, TapeView& out1,
      std::size_t blockSize) const;

  void checkFinishPositions_(
      TapeView& in0, TapeView& in1, TapeView& out0, TapeView& out1,
      std::size_t blockSize) const;

  void checkFinalPositions_(const TapeView& in0, const TapeView& in1) const;
  //                                                                          //
  //////////////////////////////////////////////////////////////////////////////

 protected:
  TapePool* tapePool_;
  //const std::size_t elementsCnt_;
  //MergeSortArithmeticsBase mergeSortCounter_;
  const std::string inFilename_;
  const bool increasing_;
  MergeSortAdditionalTapesManager tapesManager_;
};

#endif
