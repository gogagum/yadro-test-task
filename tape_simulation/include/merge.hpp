#ifndef TAPE_SIMULATION_MERGE_HPP
#define TAPE_SIMULATION_MERGE_HPP

#include <functional>

#include "impl/merge.hpp"

/**
 * @brief merge two ranges sorted increasing given by iterators to beginning and
 * sizes.
 * 
 * @tparam InputIterator1 first input iterator type.
 * @tparam InputIterator2 second input iterator type.
 * @tparam OutputIterator output iterator type.
 * @param source1 first range input iterator.
 * @param cnt1 first range length.
 * @param source2 second range input iterator.
 * @param cnt2 second range length.
 * @param target output iterator.
 * @return moved output iterator.
 */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge_increasing(InputIterator1 source1, std::size_t cnt1,
                                InputIterator2 source2, std::size_t cnt2,
                                OutputIterator target) {
  return merge<InputIterator1, InputIterator2, OutputIterator,
               std::less<>>(source1, cnt1, source2, cnt2, target);
}

/**
 * @brief merge two ranges sorted decreasing given by iterators to beginning and
 * sizes.
 * 
 * @tparam InputIterator1 first input iterator type.
 * @tparam InputIterator2 second input iterator type.
 * @tparam OutputIterator output iterator type.
 * @param source1 first range input iterator.
 * @param cnt1 first range length.
 * @param source2 second range input iterator.
 * @param cnt2 second range length.
 * @param target output iterator.
 * @return moved output iterator.
 */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge_decreasing(InputIterator1 source1, std::size_t cnt1,
                                InputIterator2 source2, std::size_t cnt2,
                                OutputIterator target) {
  return merge<InputIterator1, InputIterator2, OutputIterator,
               std::greater<>>(source1, cnt1, source2, cnt2,
                                           target);
}

#endif  // TAPE_SIMULATION_MERGE_HPP
