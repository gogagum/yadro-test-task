#ifndef TAPE_SIMULATION_MERGE_SORT_HPP
#define TAPE_SIMULATION_MERGE_SORT_HPP

#include <cassert>
#include <string>
#include <string_view>

#include "tape_pool.hpp"
#include "impl/merge_sort.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief class MergeSort - merge sort class.
class MergeSort : private MergeSortImpl {
 public:
  MergeSort(TapePool& tapePool, std::string_view inFilename,
            std::string_view tmpDirectory);

  void perform(std::string_view outFilename) &&;
};

#endif  // TAPE_SIMULATION_MERGE_SORT_HPP
