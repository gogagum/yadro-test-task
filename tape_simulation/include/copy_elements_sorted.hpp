#ifndef TAPE_SIMULATION_COPY_ELEMENTS_SORTED_HPP
#define TAPE_SIMULATION_COPY_ELEMENTS_SORTED_HPP

#include <cstdint>

#include "impl/copy_elements_sorted.hpp"

/**
 * @brief copy biggest `elementsCnt` from
 * `[source, source + elementsCnt + additionalScan)` range to position `target`
 * and after. Elements are copied in increasing order.
 *
 * @tparam InputIterator read iterator type.
 * @tparam OutputIterator write iterator type.
 * @param source position to start reading from.
 * @param target position to start writing to.
 * @param elementsCnt number of bottom elements to copy.
 * @param additionalScan number of elements scanned in addition to
 * `elementsCnt`.
 */
template <class InputIterator, class OutputIterator>
void copy_top_elements_sorted(InputIterator source, OutputIterator target,
                              std::size_t elementsCnt,
                              std::size_t additionalScan = 0) {
  copy_elements_sorted<InputIterator, OutputIterator,
                       std::greater<std::int32_t>>(source, target, elementsCnt,
                                                   additionalScan);
}

/**
 * @brief copy smallest `elementsCnt` from
 * `[source, source + elementsCnt + additionalScan)` range to position `target`
 * and after. Elements are copied in decreasing order.
 *
 * @tparam InputIterator read iterator type.
 * @tparam OutputIterator write iterator type.
 * @param source position to start reading from.
 * @param target position to start writing to.
 * @param elementsCnt number of bottom elements to copy.
 * @param additionalScan number of elements scanned in addition to
 * `elementsCnt`.
 */
template <class InputIterator, class OutputIterator>
void copy_bottom_elements_sorted(InputIterator source, OutputIterator target,
                                 std::size_t elementsCnt,
                                 std::size_t additionalScan = 0) {
  copy_elements_sorted<InputIterator, OutputIterator, std::less<std::int32_t>>(
      source, target, elementsCnt, additionalScan);
}

#endif  // TAPE_SIMULATION_COPY_ELEMENTS_SORTED_HPP
