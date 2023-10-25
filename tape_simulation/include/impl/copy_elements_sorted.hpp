#ifndef TAPE_SIMULATION_IMPL_COPY_ELEMENTS_SORTED_HPP
#define TAPE_SIMULATION_IMPL_COPY_ELEMENTS_SORTED_HPP

#include <cstdint>
#include <queue>
#include <vector>

template <class InputIterator, class OutputIterator, class Compare>
void copy_elements_sorted(InputIterator source, OutputIterator target,
                          std::size_t elementsCnt,
                          std::size_t additionalScan = 0) {
  auto q =
      std::priority_queue<std::int32_t, std::vector<std::int32_t>, Compare>();
  if (elementsCnt + additionalScan == 0) {
    return;
  }
  q.push(*source);
  ++source;
  for (std::size_t i = 1; i < elementsCnt + additionalScan; ++i) {
    if (q.size() == elementsCnt) {
      const auto val = *source;
      if (!Compare()(q.top(), val)) {
        q.pop();
        q.push(val);
      }
    } else {
      q.push(*source);
    }
    if (i + 1 != elementsCnt + additionalScan) {
      ++source;
    }
  }
  for (std::size_t i = 0; i < elementsCnt; ++i) {
    *target = q.top();
    q.pop();
    if (i + 1 != elementsCnt) {
      ++target;
    }
  }
}

#endif  // TAPE_SIMULATION_IMPL_COPY_ELEMENTS_SORTED_HPP
