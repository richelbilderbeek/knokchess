#include "game.h"

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
    m_pieces{get_starting_pieces(options)}
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

void clear_sound_effects(game& g) noexcept
{
  g.get_actions().clear_sound_effects();
}

int count_control_actions(const game& g)
{
  return count_control_actions(g.get_actions());
}

int count_piece_actions(
  const game& g,
  const chess_color player
)
{
  return count_piece_actions(g.get_pieces(), player);
}

int count_selected_units(
  const game& g,
  const chess_color player
)
{
  return count_selected_units(g.get_pieces(), player);
}

game create_king_versus_king_game()
{
  const game_options options(
    get_default_screen_size(),
    starting_position_type::kings_only,
    game_speed::normal,
    get_default_margin_width()
  );
  return game(options);
}

bool do_show_selected(const game& g) noexcept
{
  return do_show_selected(g.get_options());
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
    return g.get_player_1_pos();
  }
  assert(c == get_mouse_user_player_color(g.get_options()));
  return g.get_player_2_pos();
}

game get_default_game() noexcept
{
  return game{
    get_default_game_options()
  };
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
    return g.get_player_1_pos();
  }
  assert(get_right_player_controller(g.get_options()) == controller_type::keyboard);
  return g.get_player_2_pos();
}

game_coordinat& get_keyboard_player_pos(game& g)
{
  return g.get_keyboard_player_pos();
}

chess_color get_keyboard_user_player_color(const game& g)
{
  return get_keyboard_user_player_color(g.get_options());
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
    return g.get_player_1_pos();
  }
  assert(get_right_player_controller(g.get_options()) == controller_type::mouse);
  return g.get_player_2_pos();
}

game_coordinat& get_mouse_player_pos(game& g)
{
  return g.get_mouse_player_pos();
}

chess_color get_mouse_user_player_color(const game& g)
{
  return get_mouse_user_player_color(g.get_options());
}

game_options get_options(const game& g)
{
  return g.get_options();
}

const std::vector<piece>& get_pieces(const game& g) noexcept
{
  return g.get_pieces();
}

std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
)
{
  return get_selected_pieces(g.get_pieces(), player);
}

const std::vector<sound_effect>& get_sound_effects(const game& g) noexcept
{
  return get_sound_effects(g.get_actions());
}

const piece& get_piece_at(const game& g, const square& coordinat)
{
  return get_piece_at(g.get_pieces(), coordinat);
}

piece& get_piece_at(game& g, const square& coordinat)
{
  return get_piece_at(g.get_pieces(), coordinat);
}

bool has_selected_pieces(const game& g, const chess_color player)
{
  return !get_selected_pieces(g, player).empty();
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

void game::tick(const delta_t& dt)
{
  m_control_actions.process(*this, dt);
  for (auto& p: m_pieces) p.tick(dt);
}

void unselect_all_pieces(
  game& g,
  const chess_color color
)
{
  return unselect_all_pieces(g.get_pieces(), color);
}

void test_game() //!OCLINT tests may be many
{
#ifndef NDEBUG // no tests in release
  // Cursor positions
  {
    const auto g{create_king_versus_king_game()};
    assert(g.get_player_1_pos().get_x() >= 0.0);
    assert(g.get_player_1_pos().get_y() >= 0.0);
    assert(g.get_player_2_pos().get_x() >= 0.0);
    assert(g.get_player_2_pos().get_y() >= 0.0);
  }
  // Options, const
  {
    const auto g{get_default_game()};
    assert(g.get_options().get_left_player_color() == chess_color::white);
  }
  // Options, non-const
  {
    auto g{create_king_versus_king_game()};
    auto& options = g.get_options();
    assert(g.get_options().get_left_player_color() == chess_color::white);
    toggle_left_player_color(options);
    assert(g.get_options().get_left_player_color() == chess_color::black);
  }
  // No game actions at start
  {
    const auto g{create_king_versus_king_game()};
    assert(count_control_actions(g) == 0);
  }
  // do_show_selected
  {
    const auto g{create_king_versus_king_game()};
    assert(do_show_selected(g) || !do_show_selected(g));
  }
  // Actions in pieces accumulate
  {
    game g = create_king_versus_king_game();
    g.get_pieces().at(0).add_action(
      piece_action(
        piece_action_type::move,
        game_coordinat(1.0, 1.0)
      )
    );
    assert(count_piece_actions(g, chess_color::white) == 1);
    g.get_pieces().at(0).add_action(
      piece_action(
        piece_action_type::move,
        game_coordinat(2.0, 2.0)
      )
    );
    assert(count_piece_actions(g, chess_color::white) == 2);
  }
  {
    game g;
    assert(!is_piece_at(g, game_coordinat(3, 3)));
    for (const auto& piece: g.get_pieces())
    {
      assert(is_piece_at(g, piece.get_coordinat(), 1.0));
    }
  }
  // Clicking a unit once with LMB selects it
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Clicking a unit twice with LMB selects and unselects it
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 0);
  }
  // Clicking a unit with LMB, then another unit with LMB, only the last unit is selected
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    const auto black_queen{find_pieces(g, piece_type::queen, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    g.add_action(create_press_lmb_action(black_queen.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // LMB then RMB makes a unit move
  {
    game g = create_king_versus_king_game();
    g.add_action(create_press_lmb_action(to_coordinat("e8")));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 0);
    g.add_action(create_press_rmb_action(to_coordinat("e6")));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 1);
  }
  // 2x LMB then RMB makes a unit move 1 stretch (not 2)
  {
    game g;
    g.add_action(create_press_lmb_action(to_coordinat("e8")));
    g.add_action(create_press_rmb_action(to_coordinat("e7")));
    g.tick(delta_t(0.001));
    assert(count_piece_actions(g, chess_color::black) == 1);
    g.add_action(create_press_lmb_action(to_coordinat("e8")));
    g.add_action(create_press_rmb_action(to_coordinat("e7")));
    g.tick(delta_t(0.001));
    assert(count_piece_actions(g, chess_color::black) == 1);
  }
  // clear_sound_effects
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(get_sound_effects(g).empty());
    // Add a sound by selecting a piece
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(!get_sound_effects(g).empty());
    clear_sound_effects(g);
    assert(get_sound_effects(g).empty());
  }
  // get_cursor_pos
  {
    const game g;
    assert(get_cursor_pos(g, chess_color::white) != get_cursor_pos(g, chess_color::black));
  }
  // get_keyboard_player_pos and get_mouse_player_pos, left == keyboard
  {
    game_options options = get_default_game_options();
    options.set_left_controller_type(controller_type::keyboard);
    const game g(options);
    assert(get_keyboard_player_pos(g) != get_mouse_player_pos(g));
  }
  // get_keyboard_player_pos and get_mouse_player_pos, left == mouse
  {
    game_options options = get_default_game_options();
    options.set_left_controller_type(controller_type::mouse);
    const game g(options);
    assert(get_keyboard_player_pos(g) != get_mouse_player_pos(g));
  }
  // get_keyboard_player_pos, non-const, white == lhs == keyboard
  {
    game g;
    assert(get_keyboard_user_player_color(g) == chess_color::white);
    const auto pos_before{get_keyboard_player_pos(g)};
    auto& pos = g.get_keyboard_player_pos();
    pos += game_coordinat(0.1, 0.1);
    const auto pos_after{get_keyboard_player_pos(g)};
    assert(pos_before != pos_after);
  }
  // get_keyboard_player_pos, non-const, black == lhs == keyboard
  {
    game_options options = get_default_game_options();
    options.set_left_player_color(chess_color::black);
    options.set_left_controller_type(controller_type::keyboard);
    game g(options);
    assert(get_keyboard_user_player_color(g) == chess_color::black);
    const auto pos_before{get_keyboard_player_pos(g)};
    auto& pos = g.get_keyboard_player_pos();
    pos += game_coordinat(0.1, 0.1);
    const auto pos_after{get_keyboard_player_pos(g)};
    assert(pos_before != pos_after);
  }
  // get_mouse_player_pos
  {
    game g;
    const auto pos_before{get_mouse_player_pos(g)};
    auto& pos = g.get_mouse_player_pos();
    pos += game_coordinat(0.1, 0.1);
    const auto pos_after{get_mouse_player_pos(g)};
    assert(pos_before != pos_after);
  }
  // get_piece_at, const
  {
    const game g;
    assert(get_piece_at(g, square("e1")).get_type() == piece_type::king);
  }
  // get_piece_at, non-const
  {
    game g;
    auto& piece{get_piece_at(g, square("e1"))};
    assert(piece.get_type() == piece_type::king);
    piece.set_selected(true); // Just needs to compile
  }
  // get_pieces
  {
    const game g;
    const auto pieces{get_pieces(g)};
    const auto pieces_again{g.get_pieces()};
    assert(pieces == pieces_again);
  }
  // is_piece_at
  {
    const game g;
    assert(is_piece_at(g, square("e1")));
  }
  // toggle_left_player_color
  {
    game g;
    const auto color_before{get_left_player_color(g.get_options())};
    toggle_left_player_color(g);
    const auto color_after{get_left_player_color(g.get_options())};
    assert(color_after != color_before);
  }
  // Keyboard: select white king
  {
    game g;
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    g.get_keyboard_player_pos() = game_coordinat(white_king.get_coordinat());
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_select_action());
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // Keyboard: can move pawn forward
  {
    game g;
    g.get_keyboard_player_pos() = to_coordinat("e2");
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_select_action());
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(get_sound_effects(g).at(0).get_sound_effect_type() == sound_effect_type::select);
    g.get_keyboard_player_pos() = to_coordinat("e4");
    g.add_action(create_press_move_action());
    g.tick(); // Moves it to e3, unselects piece
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e3")).get_type() == piece_type::pawn);
    assert(get_sound_effects(g).at(1).get_sound_effect_type() == sound_effect_type::start_move);
  }
  // Keyboard: cannot move pawn backward
  {
    game g;
    g.get_keyboard_player_pos() = to_coordinat("e2");
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_select_action());
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(get_sound_effects(g).at(0).get_sound_effect_type() == sound_effect_type::select);
    g.get_keyboard_player_pos() = to_coordinat("e1");
    g.add_action(create_press_move_action());
    g.tick(); // Ignores invalid action, adds sound effect
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e2")).get_type() == piece_type::pawn);
    assert(get_sound_effects(g).at(1).get_sound_effect_type() == sound_effect_type::cannot);
  }
  // can_player_select_piece_at_cursor_pos
  {
    game g;
    assert(get_mouse_user_player_color(g) == chess_color::black);
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    g.add_action(control_action(control_action_type::mouse_move, black_king.get_coordinat()));
    g.tick();
    assert(g.get_mouse_player_pos() == black_king.get_coordinat());
    assert(can_player_select_piece_at_cursor_pos(g, chess_color::black));

  }
#endif // no tests in release
}

void toggle_left_player_color(game& g)
{
  toggle_left_player_color(g.get_options());
}
