#include "replay.h"

#include "helper.h"

#include <algorithm>
#include <cassert>

replay::replay(const std::string& pgn_str)
{
  if (pgn_str.empty()) return;

  assert(!pgn_str.empty());
  assert(std::count(std::begin(pgn_str), std::end(pgn_str), '\n') == 0);
  const std::vector<std::string> pgn_moves{
    split_pgn_str(pgn_str)
  };

  m_moves.reserve(pgn_moves.size());
  chess_color color{chess_color::white};
  for (const auto& pgn_move: pgn_moves)
  {
    m_moves.push_back(chess_move(pgn_move, color));
    color = get_other_color(color);
  }
}

int get_n_moves(const replay& r) noexcept
{
  return static_cast<int>(r.get_moves().size());
}


std::string get_replay_1_as_pgn_str() noexcept
{
  return "1. e4 e6 2. d4 b6 3. a3 Bb7 4. Nc3 Nh6 5. Bxh6 gxh6 6. Be2 Qg5 7. Bg4 h5 8. Nf3 Qg6 9. Nh4 Qg5 10. Bxh5 Qxh4 11. Qf3 Kd8 12. Qxf7 Nc6 13. Qe8# 1-0";
}

std::string get_scholars_mate_as_pgn_str() noexcept
{
  return "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0";
}

std::vector<std::string> split_pgn_str(const std::string pgn_str)
{
  std::vector<std::string> strings{
    split_str(pgn_str, ' ')
  };
  std::vector<std::string> move_strings;
  move_strings.reserve(1 + (2 * strings.size() / 3));
  std::copy_if(
    std::begin(strings),
    std::end(strings),
    std::back_inserter(move_strings),
    [](const std::string& s)
    {
      assert(!s.empty());
      return s.back() != '.';
    }
  );
  return move_strings;
}

void test_replay()
{
#ifndef NDEBUG
  // split_pgn_str
  {
    assert(split_pgn_str("1. e4").size() == 1);
  }
  // replay::replay from empty string is OK
  {
    const replay r("");
    assert(get_n_moves(r) == 0);
  }
  // replay::replay from get_scholars_mate_as_pgn_str
  {
    const replay r(get_scholars_mate_as_pgn_str());
    assert(get_n_moves(r) == 8);
  }
  // replay::replay from get_replay_1_as_pgn_str
  {
    const replay r(get_replay_1_as_pgn_str());
    assert(get_n_moves(r) > 8);
  }
#endif // NDEBUG
}
