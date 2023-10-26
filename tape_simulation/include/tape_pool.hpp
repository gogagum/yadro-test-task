#ifndef TAPE_SIMULATION_TAPE_POOL_HPP
#define TAPE_SIMULATION_TAPE_POOL_HPP

#include <cstdint>
#include <map>
#include <string>

#include "impl/tape_pool_statistics_base.hpp"
#include "tape.hpp"
#include "tape_view.hpp"

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
  [[nodiscard]] TapeView openTape(const std::string& filename);

  /**
   * @brief Create a new tape and a file for it.
   *
   * @param filename file to create.
   * @param size tape size (cells count).
   * @return a view to a created tape.
   */
  TapeView createTape(const std::string& filename, std::size_t size);

  /**
   * @brief Get view of an opened tape.
   *
   * @param filename name of a tape.
   * @return view to an opened tape.
   */
  TapeView getOpenedTape(const std::string& filename);

  /**
   * @brief Get or open tape.
   *
   * @param filename tape filename.
   * @return tape view.
   */
  TapeView getOrOpenTape(const std::string& filename);

  /**
   * @brief Remove tape.
   *
   * @param filename tape file to remove.
   */
  void removeTape(const std::string& filename);

  /**
   * @brief Close tape.
   * 
   * @param filename tape file to close.
   */
  void closeTape(const std::string& filename);

 private:
  std::map<std::string, Tape> tapes_;

 private:
  friend class TapeView;
};

#endif  // TAPE_SIMULATION_TAPE_POOL_HPP
