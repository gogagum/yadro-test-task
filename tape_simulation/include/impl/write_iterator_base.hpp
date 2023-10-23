#ifndef TAPE_SIMULATION_IMPL_WRITE_ITERATOR_BASE_HPP
#define TAPE_SIMULATION_IMPL_WRITE_ITERATOR_BASE_HPP

#include <cstdint>
#include <optional>

#include "../tape_view.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief class WriteIteratorBase
template <class Derived>
class WriteIteratorBase {
 protected:
  class WriteRef {
   public:
    explicit WriteRef(TapeView& tv);
    WriteRef& operator=(std::int32_t value);

   private:
    TapeView* tapeView_;
  };

 public:
  explicit WriteIteratorBase(TapeView& tv);
  WriteRef operator*();
  Derived operator++(int);

 protected:
  TapeView& getTapeView_();

 private:
  TapeView* tapeView_;
};

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline WriteIteratorBase<Derived>::WriteRef::WriteRef(TapeView& tv)
    : tapeView_(&tv) {
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline auto WriteIteratorBase<Derived>::WriteRef::operator=(std::int32_t value)
    -> WriteRef& {
  tapeView_->write(value);
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline WriteIteratorBase<Derived>::WriteIteratorBase(TapeView& tv)
    : tapeView_{&tv} {
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline auto WriteIteratorBase<Derived>::operator*() -> WriteRef {
  return WriteRef(getTapeView_());
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline Derived WriteIteratorBase<Derived>::operator++(int) {
  Derived ret = *static_cast<Derived*>(this);
  static_cast<Derived*>(this)->operator++();
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline TapeView& WriteIteratorBase<Derived>::getTapeView_() {
  return *tapeView_;
}

#endif  // TAPE_SIMULATION_IMPL_WRITE_ITERATOR_BASE_HPP
