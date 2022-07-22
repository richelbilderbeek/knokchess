#include "piece.h"

#include "helper.h"
#include "piece_type.h"
#include "square.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>

piece::piece(
  const chess_color color,
  const piece_type type,
  const square& coordinat,
  const side player
)
  : m_color{color},
    m_coordinat{to_coordinat(coordinat)},
    m_health{::get_max_health(type)},
    m_is_selected{false},
    m_max_health{::get_max_health(type)},
    m_player{player},
    m_type{type}
{

}

void piece::add_action(const piece_action& action)
{
  m_actions.push_back(action);
}

/// Can a piece move from 'from' to 'to'?
bool can_move(
  const piece_type& type,
  const square& from,
  const square& to,
  const side player
)
{
  switch (type)
  {
    case piece_type::king:
      return are_on_same_rank(from, to)
        || are_on_same_file(from, to)
        || are_on_same_diagonal(from, to)
      ;
    case piece_type::pawn:
      return are_on_same_file(from, to)
        && is_forward(from, to, player)
      ;
    case piece_type::rook:
      return are_on_same_rank(from, to)
        || are_on_same_file(from, to)
      ;
    case piece_type::queen:
      return are_on_same_rank(from, to)
        || are_on_same_file(from, to)
        || are_on_same_diagonal(from, to)
      ;
    case piece_type::bishop:
      return are_on_same_diagonal(from, to);
    default:
    case piece_type::knight:
      return are_on_same_half_diagonal(from, to);
  }
}

void clear_actions(piece& p)
{
  p.get_actions().clear();
  assert(count_piece_actions(p) == 0);
}

int count_piece_actions(const piece& p)
{
  return static_cast<int>(p.get_actions().size());
}

std::string describe_actions(const piece& p)
{
  const auto& actions = p.get_actions();
  if (actions.empty()) return "idle";
  std::stringstream s;
  std::transform(
    std::begin(actions),
    std::end(actions),
    std::ostream_iterator<std::string>(s, ", "),
    [](const piece_action& action) { return describe_action(action); }
  );
  std::string t = s.str();
  t.pop_back();
  t.pop_back();
  return t;
}

std::vector<piece> get_standard_starting_pieces(
  const chess_color left_player_color
) noexcept
{
  const auto f{
    left_player_color == chess_color::white
    ? [](const square& position) { return position; }
    : [](const square& position) { return get_rotated_square(position); }
  };
  const side white_side{
    left_player_color == chess_color::white
    ? side::lhs
    : side::rhs
  };
  const side black_side{get_other_side(white_side)};

  std::vector<piece> pieces{
    piece(chess_color::white, piece_type::rook,   f(square("a1")), white_side),
    piece(chess_color::white, piece_type::knight, f(square("b1")), white_side),
    piece(chess_color::white, piece_type::bishop, f(square("c1")), white_side),
    piece(chess_color::white, piece_type::queen,  f(square("d1")), white_side),
    piece(chess_color::white, piece_type::king,   f(square("e1")), white_side),
    piece(chess_color::white, piece_type::bishop, f(square("f1")), white_side),
    piece(chess_color::white, piece_type::knight, f(square("g1")), white_side),
    piece(chess_color::white, piece_type::rook,   f(square("h1")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("a2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("b2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("c2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("d2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("e2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("f2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("g2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("h2")), white_side),
    piece(chess_color::black, piece_type::rook,   f(square("a8")), black_side),
    piece(chess_color::black, piece_type::knight, f(square("b8")), black_side),
    piece(chess_color::black, piece_type::bishop, f(square("c8")), black_side),
    piece(chess_color::black, piece_type::queen,  f(square("d8")), black_side),
    piece(chess_color::black, piece_type::king,   f(square("e8")), black_side),
    piece(chess_color::black, piece_type::bishop, f(square("f8")), black_side),
    piece(chess_color::black, piece_type::knight, f(square("g8")), black_side),
    piece(chess_color::black, piece_type::rook,   f(square("h8")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("a7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("b7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("c7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("d7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("e7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("f7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("g7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("h7")), black_side)
  };
  return pieces;
}

double get_f_health(const piece& p) noexcept
{

  return p.get_health() / p.get_max_health();
}

std::vector<piece> get_kings_only_starting_pieces(
  const chess_color left_player_color
) noexcept
{
  const auto all_pieces{get_standard_starting_pieces(left_player_color)};
  std::vector<piece> pieces;
  pieces.reserve(2);
  std::copy_if(
    std::begin(all_pieces),
    std::end(all_pieces),
    std::back_inserter(pieces),
    [](const auto& piece) { return piece.get_type() == piece_type::king; }
  );
  return pieces;
}

std::vector<piece> get_starting_pieces(
  const starting_position_type t,
  const chess_color left_player_color
) noexcept
{
  switch (t)
  {
    case starting_position_type::standard: return get_standard_starting_pieces(left_player_color);
    default:
    case starting_position_type::kings_only: return get_kings_only_starting_pieces(left_player_color);
  }
}

piece get_test_piece() noexcept
{
  return piece(
    chess_color::white,
    piece_type::king,
    square("e1"),
    side::lhs
  );
}

bool has_actions(const piece& p) noexcept
{
  return count_piece_actions(p) != 0;
}

bool is_idle(const piece& p) noexcept
{
  return !has_actions(p);
}

void select(piece& p) noexcept
{
  p.set_selected(true);
}


void piece::set_selected(const bool is_selected) noexcept
{
  m_is_selected = is_selected;
}

void test_piece()
{
#ifndef NDEBUG
  // can_move
  {
    assert(can_move(piece_type::bishop, square("e4"), square("d3"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("d4"), side::lhs));
    assert(can_move(piece_type::bishop, square("e4"), square("d5"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("e3"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("e5"), side::lhs));
    assert(can_move(piece_type::bishop, square("e4"), square("f3"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("f4"), side::lhs));
    assert(can_move(piece_type::bishop, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("f6"), side::lhs));

    assert(can_move(piece_type::king, square("e4"), square("d3"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("d4"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("d5"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("e3"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("e5"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("f3"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("f4"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::king, square("e4"), square("f6"), side::lhs));

    assert(!can_move(piece_type::knight, square("e4"), square("d3"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("d4"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("d5"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("e3"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("e5"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("f3"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("f4"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("f5"), side::lhs));
    assert(can_move(piece_type::knight, square("e4"), square("f6"), side::lhs));

    assert(!can_move(piece_type::pawn, square("e4"), square("d3"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("d4"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("d5"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("e3"), side::lhs));
    assert(can_move(piece_type::pawn, square("e4"), square("e5"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("f3"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("f4"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("f6"), side::lhs));

    assert(can_move(piece_type::pawn, square("e4"), square("e3"), side::rhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("e5"), side::rhs));

    assert(can_move(piece_type::queen, square("e4"), square("d3"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("d4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("d5"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e3"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e5"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("f3"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("f4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::queen, square("e4"), square("f6"), side::lhs));

    assert(!can_move(piece_type::rook, square("e4"), square("d3"), side::lhs));
    assert(can_move(piece_type::rook, square("e4"), square("d4"), side::lhs));
    assert(!can_move(piece_type::rook, square("e4"), square("d5"), side::lhs));
    assert(can_move(piece_type::rook, square("e4"), square("e3"), side::lhs));
    assert(can_move(piece_type::rook, square("e4"), square("e5"), side::lhs));
    assert(!can_move(piece_type::rook, square("e4"), square("f3"), side::lhs));
    assert(can_move(piece_type::rook, square("e4"), square("f4"), side::lhs));
    assert(!can_move(piece_type::rook, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::rook, square("e4"), square("f6"), side::lhs));
  }
  // count_piece_actions
  {
    const auto p{get_test_piece()};
    assert(count_piece_actions(p) == 0);
  }
  // describe_actions
  {
    const auto p{get_test_piece()};
    assert(!describe_actions(p).empty());
  }
  {
    auto p{get_test_piece()};
    p.add_action(piece_action(piece_action_type::move, game_coordinat()));
    assert(!describe_actions(p).empty());
  }
  // get_health
  {
    const auto p{get_test_piece()};
    assert(p.get_health() > 0.0);
  }
  // get_f_health
  {
    const auto p{get_test_piece()};
    assert(get_f_health(p) == 1.0);
  }
  // get_max_health
  {
    const auto p{get_test_piece()};
    assert(p.get_max_health() > 0.0);
  }
  // has_actions
  {
    const auto p{get_test_piece()};
    assert(!has_actions(p));
  }
  // is_idle
  {
    const auto p{get_test_piece()};
    assert(is_idle(p));
  }
  // toggle_select
  {
    auto p{get_test_piece()};
    assert(!p.is_selected());
    toggle_select(p);
    assert(p.is_selected());
    toggle_select(p);
    assert(!p.is_selected());
  }
  // A pawn for the lhs player can move right
  {
    piece p(
      chess_color::white,
      piece_type::pawn,
      square("e2"),
      side::lhs
    );
    p.add_action(piece_action(piece_action_type::move, square("e4")));
    assert(!p.get_actions().empty());
    p.tick(delta_t(1.0));
    assert(!p.get_actions().empty());
    assert(p.get_coordinat() == get_coordinat("e3"));
  }
  // A pawn for the rhs player cannot move right
  {
    piece p(
      chess_color::white,
      piece_type::pawn,
      square("e7"),
      side::lhs
    );
    p.add_action(piece_action(piece_action_type::move, square("e5")));
    assert(!p.get_actions().empty());
    p.tick(delta_t(1.0));
    assert(p.get_actions().empty()); // Actions cleared
    assert(p.get_coordinat() == get_coordinat("e7")); // Piece stays put
  }
  // operator==
  {
    const auto a{get_test_piece()};
    const auto b{get_test_piece()};
    const piece c{chess_color::black, piece_type::pawn, game_coordinat(), side::lhs};
    assert(a == b);
    assert(!(a == c));
  }
  // operator!=
  {
    const auto a{get_test_piece()};
    const auto b{get_test_piece()};
    const piece c{chess_color::black, piece_type::pawn, game_coordinat(), side::lhs};
    assert(!(a != b));
    assert(a != c);
  }
#endif // NDEBUG
}

void piece::tick(const delta_t& dt)
{
  if (m_actions.empty()) return;
  const auto& first_action{m_actions[0]};
  if (first_action.get_type() == piece_action_type::move)
  {
    // pawns can only move forward
    if (m_type == piece_type::pawn
      && ( (m_player == side::lhs && first_action.get_coordinat().get_x() < m_coordinat.get_x())
          || (m_player == side::rhs && first_action.get_coordinat().get_x() > m_coordinat.get_x())
        )
      )
    {
      remove_first(m_actions);
      return;
    }

    const auto full_delta{first_action.get_coordinat() - m_coordinat};
    const double full_length{calc_length(full_delta)};
    if (full_length < dt.get())
    {
      // Done moving
      remove_first(m_actions);
      //std::vector<decltype(m_actions)::value_type>(m_actions.begin() + 1, m_actions.end()).swap(m_actions);
    }
    const auto delta{full_delta / (full_length / dt.get())};
    m_coordinat += delta;

  }
  else
  {
    assert(first_action.get_type() == piece_action_type::attack);
  }
}

void toggle_select(piece& p) noexcept
{
  p.set_selected(!p.is_selected());
}

void unselect(piece& p) noexcept
{
  p.set_selected(false);
}

bool operator==(const piece& lhs, const piece& rhs) noexcept
{
  return lhs.get_type() == rhs.get_type()
    && lhs.get_color() == rhs.get_color()
    && lhs.get_health() == rhs.get_health()
    && lhs.get_actions() == rhs.get_actions()
    && lhs.is_selected() == rhs.is_selected()
    && lhs.get_coordinat() == rhs.get_coordinat()
    && lhs.get_max_health() == rhs.get_max_health()
    && lhs.get_player() == rhs.get_player()
  ;
}

bool operator!=(const piece& lhs, const piece& rhs) noexcept
{
  return !(lhs == rhs);
}
