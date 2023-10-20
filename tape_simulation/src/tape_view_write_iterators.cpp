#include <tape_view_write_iterators.hpp>

RightWriteIterator& RightWriteIterator::operator=(std::int32_t x) {
  getTapeView_().write(x);
  getTapeView_().moveRight();
  return *this;
}

LeftWriteIterator& LeftWriteIterator::operator=(std::int32_t x) {
  getTapeView_().write(x);
  getTapeView_().moveLeft();
  return *this;
}
