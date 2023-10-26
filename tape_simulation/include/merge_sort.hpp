#ifndef TAPE_SIMULATION_MERGE_SORT_HPP
#define TAPE_SIMULATION_MERGE_SORT_HPP

#include <cassert>
#include <string>
#include <string_view>

#include "impl/merge_sort_impl.hpp"
#include "tape_pool.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief class MergeSort - merge sort class.
class MergeSort : private MergeSortImpl {
 public:
  MergeSort(TapePool& tapePool, std::string_view inFilename,
            std::string_view tmpDirectory, bool increasing);

  void perform(std::string_view outFilename) &&;

 private:
  void makeInitialBlocks_(TapeView& in, TapeView& out0, TapeView& out1) const;
};

#endif  // TAPE_SIMULATION_MERGE_SORT_HPP
