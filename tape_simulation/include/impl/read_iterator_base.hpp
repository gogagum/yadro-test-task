#ifndef TAPE_SIMULATION_READ_ITERATOR_BASE_HPP
#define TAPE_SIMULATION_READ_ITERATOR_BASE_HPP

#include <cstdint>
#include <optional>
#include <tape_view.hpp>

////////////////////////////////////////////////////////////////////////////////
/// \brief class ReadIteratorBase
template <class Derived>
class ReadIteratorBase {
 public:
  explicit ReadIteratorBase(TapeView& tv);
  typename std::int32_t operator*();
  bool operator==(const Derived& other) const;
  bool operator!=(const Derived& other) const;

 protected:
  [[nodiscard]] TapeView& getTapeView_() const;

 private:
  TapeView* tapeView_;
};

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline ReadIteratorBase<Derived>::ReadIteratorBase(TapeView& tv)
    : tapeView_{&tv} {
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline std::int32_t ReadIteratorBase<Derived>::operator*() {
  return tapeView_->read();
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline bool ReadIteratorBase<Derived>::operator==(const Derived& other) const {
  return other.tapeView_ == tapeView_;
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline bool ReadIteratorBase<Derived>::operator!=(const Derived& other) const {
  return other.tapeView_ != tapeView_;
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline TapeView& ReadIteratorBase<Derived>::getTapeView_() const {
  return *tapeView_;
}

#endif  // TAPE_SIMULATION_READ_ITERATOR_BASE_HPP
