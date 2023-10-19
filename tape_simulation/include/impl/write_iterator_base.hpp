#ifndef TAPE_SIMULATION_WRITE_ITERATOR_BASE_HPP
#define TAPE_SIMULATION_WRITE_ITERATOR_BASE_HPP

#include <cstdint>
#include <optional>
#include <tape_view.hpp>

////////////////////////////////////////////////////////////////////////////////
/// \brief class WriteIteratorBase
template <class Derived>
class WriteIteratorBase {
 public:
  explicit WriteIteratorBase(TapeView& tv);
  Derived& operator*();
  Derived& operator++();
  Derived operator++(int);

 protected:
  TapeView& getTapeView_();

 private:
  TapeView* tapeView_;
};

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline WriteIteratorBase<Derived>::WriteIteratorBase(TapeView& tv)
    : tapeView_{&tv} {
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline Derived& WriteIteratorBase<Derived>::operator*() {
  return static_cast<Derived&>(*this);
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline TapeView& WriteIteratorBase<Derived>::getTapeView_() {
  return *tapeView_;
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline Derived& WriteIteratorBase<Derived>::operator++() {
  return static_cast<Derived&>(*this);
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline Derived WriteIteratorBase<Derived>::operator++(int) {
  return static_cast<Derived&>(*this);
}

#endif  // TAPE_SIMULATION_WRITE_ITERATOR_BASE_HPP
