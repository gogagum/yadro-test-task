#include <tape_view_read_iterators.hpp>

////////////////////////////////////////////////////////////////////////////////
RightReadIterator& RightReadIterator::operator++() {
  getTapeView_().moveRight();
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
LeftReadIterator& LeftReadIterator::operator++() {
  getTapeView_().moveLeft();
  return *this;
}
