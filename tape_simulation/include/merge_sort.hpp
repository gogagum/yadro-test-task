#ifndef TAPE_SIMULATION_MERGE_SORT_HPP
#define TAPE_SIMULATION_MERGE_SORT_HPP

#include <string>
#include <string_view>
#include <tape_pool.hpp>
#include <tape_view.hpp>
#include <tape_view_read_iterators.hpp>
#include <tape_view_write_iterators.hpp>

////////////////////////////////////////////////////////////////////////////////
/// \brief class MergeSort - merge sort class.
class MergeSort {
 public:
  MergeSort(TapePool& tapePool, std::string_view inFilename,
            std::string_view tmpDirectory);

  void perform(std::string_view outFilename) &&;

 private:
  static std::size_t getBlockSize_(std::size_t iteration);

  [[nodiscard]] std::pair<std::size_t, std::size_t>
  getIterationsCntAndMaxBlockSize_(std::size_t startBlockSize) const;

  void makeInitialBlocks_(TapeView& in, TapeView& out0, TapeView& out1) const;

  void mergeBlocksRight_(TapeView& in0, TapeView& in1, TapeView& out0,
                         TapeView& out1, std::size_t blockSize) const;

  void mergeBlocksLeft_(TapeView& in0, TapeView& in1, TapeView& out0,
                        TapeView& out1, std::size_t blockSize) const;

  [[nodiscard]] std::size_t getBlocksCnt_(std::size_t blockSize) const;

  [[nodiscard]] std::size_t getBlocksCnt0_(std::size_t blockSize) const;

  [[nodiscard]] std::size_t getBlocksCnt1_(std::size_t blockSize) const;

  [[nodiscard]] std::pair<std::size_t, std::size_t> getBlocksCnts_(
      std::size_t blockSize) const;

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
inline std::pair<std::size_t, std::size_t> MergeSort::getBlocksCnts_(
    std::size_t blockSize) const {
  const auto blocksCnt = getBlocksCnt_(blockSize);
  return std::make_pair((blocksCnt - 1) / 2 + 1, blocksCnt / 2);
}

#endif
