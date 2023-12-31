#ifndef TAPE_SIMULATION_READ_ITERATORS_HPP
#define TAPE_SIMULATION_READ_ITERATORS_HPP

#include <iterator>

#include "impl/read_iterator_base.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief RightReadIterator - reading right iterator.
class RightReadIterator : public ReadIteratorBase<RightReadIterator> {
 public:
  using iterator_category = std::input_iterator_tag;
  using value_type = std::int32_t;
  using difference_type = void;
  using pointer = std::int32_t*;
  using reference = std::int32_t;

 public:
  using ReadIteratorBase::ReadIteratorBase;
  using ReadIteratorBase::operator*;
  RightReadIterator& operator++();
  // RightReadIterator operator++(int);
  // Postfix increment operator is created in base class from prefix increment.
};

////////////////////////////////////////////////////////////////////////////////
/// \brief RightReadIterator - reading right iterator.
class LeftReadIterator : public ReadIteratorBase<LeftReadIterator> {
 public:
  using iterator_category = std::input_iterator_tag;
  using value_type = std::int32_t;
  using difference_type = void;
  using pointer = std::int32_t*;
  using reference = std::int32_t;

 public:
  using ReadIteratorBase::ReadIteratorBase;
  using ReadIteratorBase::operator*;
  LeftReadIterator& operator++();
  // LeftReadIterator operator++(int);
  // Postfix increment operator is created in base class from prefix increment.
};

#endif  // TAPE_SIMULATION_READ_ITERATORS_HPP
