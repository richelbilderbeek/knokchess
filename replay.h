#ifndef REPLAY_H
#define REPLAY_H

#include "chess_move.h"

#include <string>
#include <vector>

/// A collection of chess moves
class replay
{
public:
  /// @param pgn_str a string of PGN notation in one line,
  /// e.g.
  replay(const std::string& pgn_str);

  const auto& get_moves() const noexcept { return m_moves; }

private:

  std::vector<chess_move> m_moves;

};

std::string get_replay_str_1() noexcept;

std::string get_scholars_mate_png_str() noexcept;

replay get_replay_1() noexcept;

/// Split the PGN string into its moves
/// E.g. '1. e4 e5 2. Nc3' will be split into {'e4', 'e5', 'Nc3'}
std::vector<std::string> split_pgn_str(const std::string pgn_str);

/// Test this class and its free functions
void test_replay();


#endif // REPLAY_H
