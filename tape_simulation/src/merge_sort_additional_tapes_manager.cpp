#include <filesystem>
#include <impl/merge_sort_additional_tapes_manager.hpp>
#include <string_view>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
MergeSortAdditionalTapesManager::MergeSortAdditionalTapesManager(
    TapePool& tapePool, std::string_view path, std::size_t tapeSize)
    : tapePool_{&tapePool},
      path_{path},
      needToRemove_{openOrCreateTmpPath_(path)},
      tmpTape00_{createTmpTape_("00", tapeSize)},
      tmpTape01_{createTmpTape_("01", tapeSize)},
      tmpTape10_{createTmpTape_("10", tapeSize)},
      tmpTape11_{createTmpTape_("11", tapeSize)} {
}

////////////////////////////////////////////////////////////////////////////////
bool MergeSortAdditionalTapesManager::openOrCreateTmpPath_(
    std::string_view tmpDirectory) {
  if (!std::filesystem::exists(tmpDirectory)) {
    std::filesystem::create_directory(tmpDirectory);
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////
TapeView MergeSortAdditionalTapesManager::createTmpTape_(
    std::string_view nameSuffix, std::size_t size) {
  std::stringstream filenameStream;
  filenameStream << path_ << "/tmp_tape_" << nameSuffix;
  return tapePool_->createTape(filenameStream.str(), size);
}

////////////////////////////////////////////////////////////////////////////////
MergeSortAdditionalTapesManager::~MergeSortAdditionalTapesManager() {
  tapePool_->removeTape(path_ + "/tmp_tape_00");
  tapePool_->removeTape(path_ + "/tmp_tape_01");
  tapePool_->removeTape(path_ + "/tmp_tape_10");
  tapePool_->removeTape(path_ + "/tmp_tape_11");

  if (needToRemove_) {
    std::filesystem::remove(path_);
  }
}
