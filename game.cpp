#include "game.h"

#include "id.h"
#include "sound_effects.h"
#include "square.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>

game::game(
  const game_options& options
)
  : m_control_actions{},
    m_layout{options.get_screen_size(), options.get_margin_width()},
    m_player_1_pos{0.5, 4.5},
    m_player_2_pos{7.5, 4.5},
    m_options{options},
    m_pieces{get_starting_pieces(options)},
    m_replayer{options.get_replayer()},
    m_t{0.0}
{

}

void game::add_action(const control_action a)
{
  // These will be processed in 'tick'
  m_control_actions.add(a);
}

bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const chess_color player
)
{
  const auto& cursor_pos{get_cursor_pos(g, player)};
  if (
    !is_piece_at(
      g,
      cursor_pos,
      g.get_options().get_click_distance()
    )
  )
  {
    return false;
  }
  const auto& piece{get_closest_piece_to(g, cursor_pos)};
  return !piece.is_selected() && piece.get_color() == player;
}

void clear_piece_messages(game& g) noexcept
{
  for (auto& p: g.get_pieces()) p.clear_messages();
}

int count_control_actions(const game& g)
{
  return count_control_actions(g.get_actions());
}

int count_piece_actions(const game& g)
{
  return count_piece_actions(g, chess_color::white)
    + count_piece_actions(g, chess_color::black)
  ;
}

int count_piece_actions(
  const game& g,
  const chess_color player
)
{
  return count_piece_actions(g.get_pieces(), player);
}

int count_selected_units(
  const game& g
)
{
  return count_selected_units(g.get_pieces());
}

int count_selected_units(
  const game& g,
  const chess_color player
)
{
  return count_selected_units(g.get_pieces(), player);
}

void game::do_move(const chess_move& m)
{
  if (is_simple_move(m))
  {
    piece& piece{get_piece_that_moves(*this, m)};
    assert(!m.get_to().empty());
    piece.set_current_square(m.get_to()[0]);
  }
  else
  {
    // Do castling, etc.
    assert(!"TODO");
  }
}

void do_move_keyboard_player_piece(game& g, const square& s)
{
  assert(count_selected_units(g, get_keyboard_user_player_color(g)) == 1);
  set_keyboard_player_pos(g, s);
  assert(square(get_keyboard_player_pos(g)) == s);
  g.add_action(create_press_move_action());
  assert(count_control_actions(g) == 1);
  g.tick(delta_t(0.1));
  assert(count_control_actions(g) == 0);
}

void do_select_and_move_keyboard_player_piece(
  game& g,
  const square& from,
  const square& to
)
{
  do_select_for_keyboard_player(g, from);
  assert(count_selected_units(g) > 0);
  do_move_keyboard_player_piece(g, to);
}

void do_select_and_start_attack_keyboard_player_piece(
  game& g,
  const square& from,
  const square& to
)
{
  do_select_for_keyboard_player(g, from);
  do_start_attack_keyboard_player_piece(g, to);
}

void do_select_for_keyboard_player(game& g, const square& s)
{
  assert(is_piece_at(g, s));
  assert(!get_piece_at(g, s).is_selected());
  set_keyboard_player_pos(g, s);
  assert(square(get_keyboard_player_pos(g)) == s);
  g.add_action(create_press_select_action());
  g.tick();
  assert(count_control_actions(g) == 0);
  assert(get_piece_at(g, s).is_selected());
}

bool do_show_selected(const game& g) noexcept
{
  return do_show_selected(g.get_options());
}

void do_start_attack_keyboard_player_piece(game& g, const square& s)
{
  assert(count_selected_units(g, get_keyboard_user_player_color(g)) == 1);
  set_keyboard_player_pos(g, s);
  assert(square(get_keyboard_player_pos(g)) == s);

  g.add_action(create_press_attack_action());
  //g.tick(delta_t(0.0)); // Process the actions
  //assert(count_control_actions(g) == 0);
}

std::vector<piece> find_pieces(
  const game& g,
  const piece_type type,
  const chess_color color
)
{
  std::vector<piece> pieces;
  std::copy_if(
    std::begin(g.get_pieces()),
    std::end(g.get_pieces()),
    std::back_inserter(pieces),
    [type, color](const auto& piece)
    {
      return piece.get_color() == color
        && piece.get_type() == type;
    }
  );
  return pieces;
}

const piece& get_closest_piece_to(
  const game& g,
  const game_coordinat& coordinat
)
{
  return g.get_pieces()[get_index_of_closest_piece_to(g, coordinat)];
}

piece& get_closest_piece_to(
  game& g,
  const game_coordinat& coordinat
)
{
  return g.get_pieces()[get_index_of_closest_piece_to(g, coordinat)];
}

game_coordinat get_cursor_pos(
  const game& g,
  const chess_color c
)
{
  if (c == get_keyboard_user_player_color(g.get_options()))
  {
    return get_player_pos(g, side::lhs);
  }
  assert(c == get_mouse_user_player_color(g.get_options()));
  return get_player_pos(g, side::rhs);
}

game get_default_game() noexcept
{
  return game{
    get_default_game_options()
  };
}

game get_game_with_starting_position(starting_position_type t) noexcept
{
  const game_options options(
    get_default_screen_size(),
    t,
    get_default_game_speed(),
    get_default_margin_width()
  );
  return game(options);
}

id get_id(const game& g, const square& s)
{
  assert(is_piece_at(g, s));
  return get_piece_at(g, s).get_id();
}

int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinat& coordinat
)
{
  const std::vector<double> distances{
    calc_distances(g.get_pieces(), coordinat)
  };
  const auto iter = std::min_element(std::begin(distances), std::end(distances));
  const auto index = std::distance(std::begin(distances), iter);
  return index;
}

game_coordinat& game::get_keyboard_player_pos()
{
  if (get_left_player_controller(m_options) == controller_type::keyboard)
  {
    return m_player_1_pos;
  }
  assert(get_right_player_controller(m_options) == controller_type::keyboard);
  return m_player_2_pos;
}

game_coordinat get_keyboard_player_pos(const game& g)
{
  if (get_left_player_controller(g.get_options()) == controller_type::keyboard)
  {
    return get_player_pos(g, side::lhs);
  }
  assert(get_right_player_controller(g.get_options()) == controller_type::keyboard);
  return get_player_pos(g, side::rhs);
}

game_coordinat& get_keyboard_player_pos(game& g)
{
  return g.get_keyboard_player_pos();
}

chess_color get_keyboard_user_player_color(const game& g)
{
  return get_keyboard_user_player_color(g.get_options());
}

game get_kings_only_game() noexcept
{
  return get_game_with_starting_position(starting_position_type::kings_only);
}

const game_view_layout& get_layout(const game& g) noexcept
{
  return g.get_layout();
}

game_coordinat& game::get_mouse_player_pos()
{
  if (get_left_player_controller(m_options) == controller_type::mouse)
  {
    return m_player_1_pos;
  }
  assert(get_right_player_controller(m_options) == controller_type::mouse);
  return m_player_2_pos;
}

game_coordinat get_mouse_player_pos(const game& g)
{
  if (get_left_player_controller(g.get_options()) == controller_type::mouse)
  {
    return get_player_pos(g, side::lhs);
  }
  assert(get_right_player_controller(g.get_options()) == controller_type::mouse);
  return get_player_pos(g, side::rhs);
}

game_coordinat& get_mouse_player_pos(game& g)
{
  return g.get_mouse_player_pos();
}

chess_color get_mouse_user_player_color(const game& g)
{
  return get_mouse_user_player_color(g.get_options());
}

double get_music_volume_as_percentage(const game& g) noexcept
{
  return get_music_volume_as_percentage(g.get_options());
}

std::vector<square> get_occupied_squares(const game& g) noexcept
{
  return get_occupied_squares(get_pieces(g));
}

const game_options& get_options(const game& g)
{
  return g.get_options();
}

std::vector<piece>& get_pieces(game& g) noexcept
{
  return g.get_pieces();
}

const std::vector<piece>& get_pieces(const game& g) noexcept
{
  return g.get_pieces();
}

piece& get_piece_that_moves(game& g, const chess_move& move)
{
  assert(is_simple_move(move));
  assert(!g.get_pieces().empty());
  const auto& pieces{g.get_pieces()};
  const int n_pieces{static_cast<int>(pieces.size())};
  for (int i{0}; i!=n_pieces; ++i)
  {
    auto& ps{g.get_pieces()};
    auto& piece{ps[i]};
    if (piece.get_color() != move.get_color()) continue;
    const auto& color{piece.get_color()};
    assert(move.get_type().size() == 1);
    if (piece.get_type() != move.get_type()[0]) continue;
    const auto& piece_type{piece.get_type()};
    const auto& from{piece.get_current_square()};
    assert(move.get_to().size() == 1);
    const auto& to{move.get_to()[0]};
    const side& player{get_player_side(g, color)};
    if (can_move(piece_type, from, to, player))
    {
      return piece;
    }
  }
  assert(!"Should not het here");
}


piece get_piece_with_id(
  const game& g,
  const id& i
)
{
  return get_piece_with_id(g.get_pieces(), i);
}

chess_color get_player_color(
  const game& g,
  const side player
) noexcept
{
  return get_player_color(g.get_options(), player);
}

side get_player_side(const game& g, const chess_color& color) noexcept
{
  if (get_player_color(g, side::lhs) == color) return side::lhs;
  assert(get_player_color(g, side::rhs) == color);
  return side::rhs;
}

const game_coordinat& game::get_player_pos(const side player) const noexcept
{
  if (player == side::lhs) return m_player_1_pos;
  assert(player == side::rhs);
  return m_player_2_pos;
}

const game_coordinat& get_player_pos(const game& g, const side player) noexcept
{
  return g.get_player_pos(player);
}

std::vector<square> get_possible_moves(
  const game& g,
  const side player
)
{
  const auto selected_pieces{get_selected_pieces(g, player)};
  if (selected_pieces.empty()) return {};
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};
  return get_possible_moves(
    get_pieces(g),
    selected_piece
  );
}

std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
)
{
  return get_selected_pieces(g.get_pieces(), player);
}

std::vector<piece> get_selected_pieces(
  const game& g,
  const side player
)
{
  return get_selected_pieces(g.get_pieces(), get_player_color(g, player));
}

std::vector<message> collect_messages(const game& g) noexcept
{
  const auto& pieces{g.get_pieces()};
  std::vector<message> effects;
  for (const auto& piece: pieces)
  {
    const auto& es{piece.get_messages()};
    std::transform(
      std::begin(es),
      std::end(es),
      std::back_inserter(effects),
      [piece](const message_type type)
      {
        return message(
          type,
          piece.get_color(),
          piece.get_type()
        );
      }
    );
  }
  return effects;
}

const piece& get_piece_at(const game& g, const square& coordinat)
{
  return get_piece_at(g.get_pieces(), coordinat);
}

piece& get_piece_at(game& g, const square& coordinat)
{
  assert(is_piece_at(g, coordinat));
  return get_piece_at(g.get_pieces(), coordinat);
}

const delta_t& get_time(const game& g) noexcept
{
  return g.get_time();
}

bool has_selected_pieces(const game& g, const chess_color player)
{
  return !get_selected_pieces(g, player).empty();
}

bool is_idle(const game& g) noexcept
{
  return count_piece_actions(g) == 0;
}

bool is_piece_at(
  const game& g,
  const game_coordinat& coordinat,
  const double distance
) {
  return is_piece_at(g.get_pieces(), coordinat, distance);
}

bool is_piece_at(
  const game& g,
  const square& coordinat
) {
  return is_piece_at(g.get_pieces(), coordinat);
}

bool piece_with_id_is_at(
  game& g,
  const id& i,
  const square& s
)
{
  assert(is_piece_at(g, s));
  return get_piece_at(g, s).get_id() == i;
}

void set_keyboard_player_pos(
  game& g,
  const square& s
)
{
  const auto current_pos{get_keyboard_player_pos(g)};
  const int n_right{(s.get_x() - square(current_pos).get_x() + 8) % 8};
  const int n_down{(s.get_y() - square(current_pos).get_y() + 8) % 8};
  for (int i{0}; i!=n_right; ++i)
  {
    g.add_action(create_press_right_action());
  }
  for (int i{0}; i!=n_down; ++i)
  {
    g.add_action(create_press_down_action());
  }
  g.tick();
  assert(count_control_actions(g) == 0);
}

void game::tick(const delta_t& dt)
{
  // Let the replayer do its move
  m_replayer.do_move(*this);

  // Convert control_actions to piece_actions instantaneous
  m_control_actions.process(*this);

  assert(count_dead_pieces(m_pieces) == 0);

  // Do those piece_actions
  for (auto& p: m_pieces) p.tick(dt, *this);

  // Remove dead pieces
  m_pieces.erase(
    std::remove_if(
      std::begin(m_pieces),
      std::end(m_pieces),
      [](const auto& p) { return is_dead(p); }
    ),
    std::end(m_pieces)
  );
  assert(count_dead_pieces(m_pieces) == 0);

  // Keep track of the time
  m_t += dt;
}

void unselect_all_pieces(
  game& g,
  const chess_color color
)
{
  return unselect_all_pieces(g.get_pieces(), color);
}

void tick_until_idle(game& g)
{
  int cnt{0};
  while (!is_idle(g))
  {
    g.tick(delta_t(0.1));
    ++cnt;
    assert(cnt < 1000);
  }
}

void toggle_left_player_color(game& g)
{
  toggle_left_player_color(g.get_options());
}

