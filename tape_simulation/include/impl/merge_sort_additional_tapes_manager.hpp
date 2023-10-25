#ifndef TAPE_SIMULATION_IMPL_MERGE_SORT_ADDITIONAL_TAPES_MANAGER_HPP
#define TAPE_SIMULATION_IMPL_MERGE_SORT_ADDITIONAL_TAPES_MANAGER_HPP

#include <string_view>

#include "../tape_pool.hpp"

class MergeSortAdditionalTapesManager {
 public:
  MergeSortAdditionalTapesManager(std::string_view path, std::size_t tapeSize);
  MergeSortAdditionalTapesManager(const MergeSortAdditionalTapesManager&) =
      delete;
  MergeSortAdditionalTapesManager(MergeSortAdditionalTapesManager&&) noexcept =
      delete;
  auto operator=(const MergeSortAdditionalTapesManager&) = delete;
  auto operator=(MergeSortAdditionalTapesManager&&) noexcept = delete;

  TapeView createTmpTape_(std::string_view name, std::size_t size);

  TapeView& getInTape0(std::size_t iterationIdx);

  TapeView& getInTape1(std::size_t iterationIdx);

  TapeView& getOutTape0(std::size_t iterationIdx);

  TapeView& getOutTape1(std::size_t iterationIdx);

  TapeView& getInitialOutTape0();

  TapeView& getInitialOutTape1();

  static bool openOrCreateTmpPath_(std::string_view tmpDirectory);

  ~MergeSortAdditionalTapesManager();

 private:
  TapePool tapePool_{};
  const std::string path_;
  const bool needToRemove_;
  TapeView tmpTape00_;
  TapeView tmpTape01_;
  TapeView tmpTape10_;
  TapeView tmpTape11_;
};

////////////////////////////////////////////////////////////////////////////////
inline TapeView& MergeSortAdditionalTapesManager::getInTape0(
    std::size_t iterationIdx) {
  return (iterationIdx % 2 == 0) ? tmpTape00_ : tmpTape10_;
}

////////////////////////////////////////////////////////////////////////////////
inline TapeView& MergeSortAdditionalTapesManager::getInTape1(
    std::size_t iterationIdx) {
  return (iterationIdx % 2 == 0) ? tmpTape01_ : tmpTape11_;
}

////////////////////////////////////////////////////////////////////////////////
inline TapeView& MergeSortAdditionalTapesManager::getOutTape0(
    std::size_t iterationIdx) {
  return (iterationIdx % 2 == 0) ? tmpTape10_ : tmpTape00_;
}

////////////////////////////////////////////////////////////////////////////////
inline TapeView& MergeSortAdditionalTapesManager::getOutTape1(
    std::size_t iterationIdx) {
  return (iterationIdx % 2 == 0) ? tmpTape11_ : tmpTape01_;
}

////////////////////////////////////////////////////////////////////////////////
inline TapeView& MergeSortAdditionalTapesManager::getInitialOutTape0() {
  return tmpTape00_;
}

////////////////////////////////////////////////////////////////////////////////
inline TapeView& MergeSortAdditionalTapesManager::getInitialOutTape1() {
  return tmpTape01_;
}

#endif  // TAPE_SIMULATION_IMPL_MERGE_SORT_ADDITIONAL_TAPES_MANAGER_HPP
