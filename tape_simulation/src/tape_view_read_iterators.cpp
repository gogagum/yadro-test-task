#include <tape_view_read_iterators.hpp>

////////////////////////////////////////////////////////////////////////////////
RightReadIterator& RightReadIterator::operator++() {
  getTapeView_().moveRight();
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
RightReadIterator RightReadIterator::operator++(int) {
  auto ret = *this;
  getTapeView_().moveRight();
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
LeftReadIterator& LeftReadIterator::operator++() {
  getTapeView_().moveLeft();
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
LeftReadIterator LeftReadIterator::operator++(int) {
  auto ret = *this;
  getTapeView_().moveLeft();
  return ret;
}
