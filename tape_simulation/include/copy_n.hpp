#ifndef TAPES_SIMULATION_COPY_N_HPP
#define TAPES_SIMULATION_COPY_N_HPP

#include <cstdint>
#include <sstream>
#include <stdexcept>

/**
 * @brief Copy n elements. Moves both input and output iterator n times.
 * Copy function expects at least one element to be copied. Otherwise it throws
 * an exception.
 *
 * @tparam InputIterator input iterator type.
 * @tparam OutputIterator output iterator type.
 * @param firstIn iterator to a beginning of a copied range.
 * @param n number of elements to be copied.
 * @param firstOut iterator to output beginning.
 * @return iterator to last copied element.
 */
template <class InputIterator, class OutputIterator>
OutputIterator copy_n(InputIterator firstIn, std::size_t n,
                      OutputIterator firstOut) {
  if (n == 0) {
    std::stringstream messageStream;
    messageStream << "Tried copying zero elements.";
    throw std::logic_error(messageStream.str());
  }

  for (std::size_t i = 0; i < n - 1; ++i, ++firstIn, ++firstOut) {
    *firstOut = *firstIn;
  }
  *firstOut = *firstIn;

  return firstOut;
}

#endif  // TAPES_SIMULATION_COPY_N_HPP
