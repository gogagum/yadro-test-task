#ifndef TAPE_SIMULATION_WRITE_ITERATORS_HPP
#define TAPE_SIMULATION_WRITE_ITERATORS_HPP

#include <iterator>
#include <tape_view.hpp>

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
  RightWriteIterator& operator=(std::int32_t x);
  RightWriteIterator& operator++();
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
  LeftWriteIterator& operator=(std::int32_t x);
  LeftWriteIterator& operator++();
};

#endif  // TAPE_SIMULATION_WRITE_ITERATORS_HPP
