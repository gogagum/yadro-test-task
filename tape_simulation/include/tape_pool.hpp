#ifndef TAPE_SIMULATION_TAPE_POOL_HPP
#define TAPE_SIMULATION_TAPE_POOL_HPP

#include <cstdint>
#include <map>
#include <string>
#include <tape.hpp>
#include <tape_view.hpp>

#include "impl/tape_pool_statistics_base.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief class TapePool - tape views fabric, which counts operations.
class TapePool : public TapePoolStatisticsBase {
 public:
  using IOStatistics = TapePoolStatisticsBase::IOStatistics;

 public:
  TapePool() = default;

  /**
   * @brief Open existing tape from existing file. Tape size depends on a size
   * of existing file.
   *
   * @param filename file to open.
   * @return a view to an opened tape.
   */
  [[nodiscard]] TapeView openTape(std::string_view filename);

  /**
   * @brief Create a new tape and a file for it.
   *
   * @param filename file to create.
   * @param size tape size (cells count).
   * @return a view to a created tape.
   */
  TapeView createTape(std::string_view filename, std::size_t size);

  /**
   * @brief Get view of an opened tape.
   *
   * @param filename name of a tape.
   * @return view to an opened tape.
   */
  TapeView getOpenedTape(std::string_view filename);

  /**
   * @brief Get or open tape.
   * 
   * @param filename tape filename.
   * @return tape view.
   */
  TapeView getOrOpenTape(std::string_view filename);

  /**
   * @brief Remove tape.
   *
   * @param filename tape file to remove.
   */
  void removeTape(std::string_view filename);

 private:
  std::map<std::string_view, Tape> tapes_;

 private:
  friend class TapeView;
};

#endif
