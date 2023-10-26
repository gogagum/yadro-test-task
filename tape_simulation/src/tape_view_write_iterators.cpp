#include <tape_view_write_iterators.hpp>

////////////////////////////////////////////////////////////////////////////////
RightWriteIterator& RightWriteIterator::operator++() {
  getTapeView_().moveRight();
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
LeftWriteIterator& LeftWriteIterator::operator++() {
  getTapeView_().moveLeft();
  return *this;
}
