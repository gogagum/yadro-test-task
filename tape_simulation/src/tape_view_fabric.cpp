#include <sstream>
#include <stdexcept>
#include <tape_view.hpp>
#include <tape_view_fabric.hpp>

////////////////////////////////////////////////////////////////////////////////
TapeView& TapeViewFabric::openTape(std::string_view filename) {
  increaseOpenCnt();
  return openedViews_.at(filename);
}

////////////////////////////////////////////////////////////////////////////////
TapeView& TapeViewFabric::createTape(std::string_view filename,
                                     std::size_t size) {
  increaseCreateCnt();
  if (openedViews_.find(filename) != openedViews_.end()) {
    std::stringstream messageStream;
    messageStream << "The tape with such name(" << filename
                  << ") already exists.";
    throw std::logic_error(messageStream.str());
  }
  openedViews_.emplace(filename, TapeView::createNew_(*this, filename, size));
  return openedViews_.at(filename);
}

////////////////////////////////////////////////////////////////////////////////
void TapeViewFabric::removeTape(std::string_view filename) {
  increaseRemoveCnt();
  openedViews_.erase(filename);
}
