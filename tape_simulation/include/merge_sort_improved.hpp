#ifndef TAPE_SIMULATION_MERGE_SORT_IMPROVED_HPP
#define TAPE_SIMULATION_MERGE_SORT_IMPROVED_HPP

#include <cassert>
#include <string>
#include <string_view>

#include "impl/merge_sort_impl.hpp"
#include "tape_pool.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief class MergeSort - merge sort class.
class MergeSortImproved : private MergeSortImpl {
 public:
  class ZeroHeapSizeLimit : std::logic_error {
   public:
    ZeroHeapSizeLimit();
  };

 public:
  MergeSortImproved(TapePool& tapePool, std::string_view inFilename,
                    std::string_view tmpDirectory, bool increasing,
                    std::size_t heapSizeLimit);

  void perform(std::string_view outFilename) &&;

 private:
  void makeInitialBlocks_(TapeView& in, TapeView& out0, TapeView& out1) const;

  static void copyElementsSorted_(RightReadIterator read,
                                  RightWriteIterator write, std::size_t cnt,
                                  bool increasing);
};

#endif  // TAPE_SIMULATION_MERGE_SORT_HPP
