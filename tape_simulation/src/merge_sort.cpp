#include <array>
#include <cassert>
#include <filesystem>
#include <merge_sort.hpp>
#include <tape_pool.hpp>

#include "merge.hpp"
#include "tape_view_read_iterators.hpp"
#include "tape_view_write_iterators.hpp"

////////////////////////////////////////////////////////////////////////////////
MergeSort::MergeSort(TapePool& tapePool, std::string_view inFilename,
                     std::string_view tmpDirectory)
    : MergeSortImpl(tapePool, inFilename, tmpDirectory) {
}

////////////////////////////////////////////////////////////////////////////////
void MergeSort::perform(std::string_view outFilename) && {
  auto inTape = getTapePool_().getOpenedTape(getInFilename_());
  auto outTape = getTapePool_().createTape(outFilename, getElementsCnt_());

  if (getElementsCnt_() == 0) {
    return;
  }

  if (getElementsCnt_() == 1) {
    outTape.write(inTape.read());
    return;
  }

  const auto [iterationsCnt, maxBlockSize] =
      getIterationsCntAndMaxBlockSize_(1);

  const std::string tmpTapeName00 = getTmpDirectoryName_() + "/tmpTape00";
  const std::string tmpTapeName01 = getTmpDirectoryName_() + "/tmpTape01";
  const std::string tmpTapeName10 = getTmpDirectoryName_() + "/tmpTape10";
  const std::string tmpTapeName11 = getTmpDirectoryName_() + "/tmpTape11";

  auto tape00 = getTapePool_().createTape(tmpTapeName00, maxBlockSize);
  auto tape01 = getTapePool_().createTape(tmpTapeName01, maxBlockSize);
  auto tape10 = getTapePool_().createTape(tmpTapeName10, maxBlockSize);
  auto tape11 = getTapePool_().createTape(tmpTapeName11, maxBlockSize);

  makeInitialBlocks_(inTape, tape00, tape01);

  std::size_t iterationsLeft = iterationsCnt;
  std::size_t blockSize = 1;

  for (; iterationsLeft > 0; --iterationsLeft, blockSize *= 2) {
    std::size_t iterationIdx = iterationsCnt - iterationsLeft;
    auto& in0 = (iterationIdx % 2 == 0) ? tape00 : tape10;
    auto& in1 = (iterationIdx % 2 == 0) ? tape01 : tape11;
    auto& out0 = (iterationIdx % 2 == 0) ? tape10 : tape00;
    auto& out1 = (iterationIdx % 2 == 0) ? tape11 : tape01;

    if (iterationsLeft % 2 == 1) {
      mergeBlocks1_(in0, in1, out0, out1, blockSize);
    } else {
      mergeBlocks0_(in0, in1, out0, out1, blockSize);
    }
  }

  auto& inTape0 = (iterationsCnt % 2 == 0) ? tape00 : tape10;
  auto& inTape1 = (iterationsCnt % 2 == 0) ? tape01 : tape11;

  assert(inTape0.getPosition() + 1 == maxBlockSize &&
         "First final tape must contain full max block.");
  assert(inTape1.getPosition() + 1 == getElementsCnt_() - maxBlockSize &&
         "Second final tape must contain rest elements.");

  auto in0 = LeftReadIterator(inTape0);
  auto in1 = LeftReadIterator(inTape1);
  auto out = RightWriteIterator(outTape);

  merge_increasing(in0, maxBlockSize, in1, getElementsCnt_() - maxBlockSize,
                   out);

  getTapePool_().removeTape(tmpTapeName00);
  getTapePool_().removeTape(tmpTapeName01);
  getTapePool_().removeTape(tmpTapeName10);
  getTapePool_().removeTape(tmpTapeName11);

  if (needToRemoveTmpDirectory_()) {
    std::filesystem::remove(getTmpDirectoryName_());
  }
}
