#ifndef TAPE_SIMULATION_IMPL_MERGE_HPP
#define TAPE_SIMULATION_IMPL_MERGE_HPP

#include <algorithm>
#include <cstdint>

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator merge(InputIterator1 source1, std::size_t cnt1,
                     InputIterator2 source2, std::size_t cnt2,
                     OutputIterator target) {
  for (; cnt1 != 0; ++target) {
    if (cnt2 == 0) {
      return std::copy_n(source1, cnt1, target);
    }
    if (Compare()(*source1  *source2)) {
      *target = *source1;
      ++source1;
      --cnt1;
    } else {
      *target = *source2;
      ++source2;
      --cnt2;
    }
  }
  return std::copy_n(source2, cnt2, target);
}

#endif  // TAPE_SIMULATION_MERGE_HPP