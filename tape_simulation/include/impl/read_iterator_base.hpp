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
  TapeView& getTapeView_();

  void resetCache_();

 private:
  TapeView* tapeView_;
  std::optional<std::uint32_t> readCache_{};
};

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline ReadIteratorBase<Derived>::ReadIteratorBase(TapeView& tv)
    : tapeView_{&tv} {
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline std::int32_t ReadIteratorBase<Derived>::operator*() {
  if (!readCache_.has_value()) {
    readCache_ = tapeView_->read();
  }
  return *readCache_;
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
inline TapeView& ReadIteratorBase<Derived>::getTapeView_() {
  return *tapeView_;
}

////////////////////////////////////////////////////////////////////////////////
template <class Derived>
inline void ReadIteratorBase<Derived>::resetCache_() {
  readCache_.reset();
}

#endif  // TAPE_SIMULATION_READ_ITERATOR_BASE_HPP
