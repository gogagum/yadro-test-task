#ifndef TAPE_SIMULATION_WRITE_ITERATORS_HPP
#define TAPE_SIMULATION_WRITE_ITERATORS_HPP

#include <iterator>

#include "impl/write_iterator_base.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief RightWriteIterator - writing to right iterator. 
class RightWriteIterator : public WriteIteratorBase<RightWriteIterator> {
 public:
  using iterator_category = std::output_iterator_tag;
  using value_type = void;
  using difference_type = void;
  using pointer = void;
  using reference = WriteIteratorBase<RightWriteIterator>::WriteRef;

 public:
  using WriteIteratorBase::WriteIteratorBase;
  RightWriteIterator& operator++();
  using WriteIteratorBase::operator++; // Postfix iterator
};

////////////////////////////////////////////////////////////////////////////////
/// \brief RightWriteIterator - writing to left iterator.
class LeftWriteIterator : public WriteIteratorBase<LeftWriteIterator> {
 public:
  using iterator_category = std::output_iterator_tag;
  using value_type = void;
  using difference_type = void;
  using pointer = void;
  using reference = WriteIteratorBase<LeftWriteIterator>::WriteRef;

 public:
  using WriteIteratorBase::WriteIteratorBase;
  LeftWriteIterator& operator++();
  using WriteIteratorBase::operator++;
};

#endif  // TAPE_SIMULATION_WRITE_ITERATORS_HPP
