#include <tape_view_read_iterators.hpp>

////////////////////////////////////////////////////////////////////////////////
RightReadIterator& RightReadIterator::operator++() {
  getTapeView_().moveRight();
  resetCache_();
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
RightReadIterator RightReadIterator::operator++(int) {
  auto ret = *this;
  getTapeView_().moveRight();
  resetCache_();
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
LeftReadIterator& LeftReadIterator::operator++() {
  getTapeView_().moveLeft();
  resetCache_();
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
LeftReadIterator LeftReadIterator::operator++(int) {
  auto ret = *this;
  getTapeView_().moveLeft();
  resetCache_();
  return ret;
}
