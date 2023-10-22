#ifndef TAPE_SIMULATION_IMPL_MERGE_HPP
#define TAPE_SIMULATION_IMPL_MERGE_HPP

#include <algorithm>
#include <copy_n.hpp>
#include <cstdint>
#include <optional>
#include <cassert>

/**
 * @brief merge two ranges of lengths `cnt0` and `cnt1` doing `cnt0 + cnt1`
 * reads (dereferences), `cnt0 - 1` first input iterator increments,
 * `cnt1 - 1` second iterator increments and `cnt0 + cnt1 - 1` output iterator
 * increments. Returns output iterator to a last output element.
 * 
 * @tparam InputIterator1 
 * @tparam InputIterator2 
 * @tparam OutputIterator 
 * @tparam Compare 
 * @param source0 
 * @param cnt0 
 * @param source1 
 * @param cnt1 
 * @param target 
 * @return OutputIterator 
 */
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator merge(InputIterator1 source0, std::size_t cnt0,
                     InputIterator2 source1, std::size_t cnt1,
                     OutputIterator target) {
  if (cnt0 == 0 && cnt1 == 0) {
    throw std::logic_error("Tried merging two empty sources.");
  }

  if (cnt0 == 0 || cnt1 == 0) {
    std::stringstream messageStream;
    messageStream << "Tried merging with empty: "
                  << ((cnt0 == 0) ? "cnt0" : "cnt1") << " == 0.";
    throw std::logic_error(messageStream.str());
  }

  auto lastRead1 = *source1;
  if (--cnt1; cnt1 > 0) {
    ++source1;
  }

  for (; cnt0 > 0;) {
    auto current0 = *source0;
    while (Compare()(lastRead1, current0)) {
      *target = lastRead1;
      ++target;
      if (cnt1 == 0) {
        *target = current0;
        if (--cnt0; cnt0 > 0) {
          ++source0;
          ++target;
          return copy_n(source0, cnt0, target);
        }
        return target;
      }
      lastRead1 = *source1;
      if (--cnt1; cnt1 > 0) {
        ++source1;
      }
    }
    *target = current0;
    ++target;
    if (--cnt0; cnt0 > 0) {
      ++source0;
    }
  }

  *target = lastRead1;
  if (cnt1 > 0) {
    ++target;
    return copy_n(source1, cnt1, target);
  }

  return target;
}

#endif  // TAPE_SIMULATION_MERGE_HPP