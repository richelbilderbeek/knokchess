#include "game.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>

game::game(
  const game_options& options
)
  : m_layout{options.get_screen_size(), options.get_margin_width()},
    m_player_1_pos{0.5, 4.5},
    m_player_2_pos{7.5, 4.5},
    m_options{options},
    m_pieces{get_starting_pieces(options)}
{

}

void game::add_action(const control_action a)
{
  // These will be processed in 'tick'
  m_actions.push_back(a);
}

std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
) {
  std::vector<double> distances;
  distances.reserve(pieces.size());
  std::transform(
    std::begin(pieces),
    std::end(pieces),
    std::back_inserter(distances),
    [coordinat](const auto& piece) {
      return calc_distance(
        coordinat,
        piece.get_coordinat()
      );
    }
  );
  assert(distances.size() == pieces.size());
  return distances;
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
  const auto& piece{g.get_closest_piece_to(cursor_pos)};
  return !piece.is_selected() && piece.get_color() == player;
}

void game::clear_sound_effects() noexcept
{
  m_sound_effects.clear();
  assert(m_sound_effects.empty());
}

int count_game_actions(const game& g)
{
  return static_cast<int>(g.get_actions().size());
}

int count_piece_actions(
  const game& g,
  const chess_color player
)
{
  const auto& pieces{g.get_pieces()};
  return std::accumulate(
    std::begin(pieces),
    std::end(pieces),
    0,
    [player](const int n, const auto& piece)
    {
      if (piece.get_color() == player)
      {
        return n + count_piece_actions(piece);
      }
      return n;
    }
  );
}

int count_selected_units(
  const game& g,
  const chess_color player
)
{
  return std::count_if(
    std::begin(g.get_pieces()),
    std::end(g.get_pieces()),
    [player](const auto& piece)
    {
      return piece.is_selected()
        && piece.get_color() == player;
    }
  );
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

void game::do_select(
  const game_coordinat& coordinat,
  const chess_color player_color
)
{
  // #|Has selected pieces? |Clicking on what?|Do what?
  // -|---------------------|-----------------|----------------
  // 1|Yes                  |Selected unit    |Unselect unit
  // 2|Yes                  |Unselected unit  |Select unit
  // 3|Yes                  |Empty square     |Unselect all units
  // 4|No                   |Selected unit    |NA
  // 5|No                   |Unselected unit  |Select unit
  // 6|No                   |Empty square     |Nothing
  if (has_selected_pieces(*this, player_color))
  {
    if (is_piece_at(*this, coordinat)) {
      auto& piece{get_closest_piece_to(coordinat)};
      if (piece.get_color() == player_color)
      {
        if (piece.is_selected())
        {
          unselect(piece); // 1
        }
        else
        {
          unselect_all_pieces(*this, player_color);
          select(piece); // 2
          m_sound_effects.push_back(
            sound_effect(
              sound_effect_type::select,
              piece.get_color(),
              piece.get_type()
            )
          );
        }
      }
    }
    else
    {
      unselect_all_pieces(*this, player_color); // 3
    }
  }
  else
  {
    if (is_piece_at(*this, coordinat)) {
      auto& piece{get_closest_piece_to(coordinat)};
      if (piece.get_color() == player_color)
      {
        if (piece.is_selected())
        {
          assert(!"Should never happen, as there are no selected pieces at all");
          unselect(piece); // 4
        }
        else
        {
          select(piece); // 5
          m_sound_effects.push_back(
            sound_effect(
              sound_effect_type::select,
              piece.get_color(),
              piece.get_type()
            )
          );
        }
      }
    }
    else
    {
      // 6
    }
  }
}

bool do_show_selected(const game& g) noexcept
{
  return do_show_selected(g.get_options());
}

void game::start_move_unit(
  const game_coordinat& coordinat,
  const chess_color player_color
)
{
  for (auto& p: m_pieces)
  {
    if (p.is_selected() && p.get_color() == player_color)
    {
      // No shift, so all current actions are void
      clear_actions(p);

      p.add_action(
        piece_action(
          piece_action_type::move,
          center_on_center(coordinat)
        )
      );

      m_sound_effects.push_back(
        sound_effect(
          sound_effect_type::start_move,
          p.get_color(),
          p.get_type()
        )
      );
    }
  }
  unselect_all_pieces(*this, player_color);
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

const piece& game::get_closest_piece_to(const game_coordinat& coordinat) const
{
  return m_pieces[get_index_of_closest_piece_to(*this, coordinat)];
}

piece& game::get_closest_piece_to(const game_coordinat& coordinat)
{
  return m_pieces[get_index_of_closest_piece_to(*this, coordinat)];
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

std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
)
{
  std::vector<piece> pieces;
  const auto& all_pieces = g.get_pieces();
  std::copy_if(
    std::begin(all_pieces),
    std::end(all_pieces),
    std::back_inserter(pieces),
    [player](const auto& piece)
    {
      return piece.is_selected() && piece.get_color() == player;
    }
  );
  return pieces;
}

const std::vector<piece>& get_pieces(const game& g) noexcept
{
  return g.get_pieces();
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
  const std::vector<double> distances{
    calc_distances(g.get_pieces(), coordinat)
  };
  const auto iter = std::find_if(
    std::begin(distances),
    std::end(distances),
    [distance](const double this_distance) { return this_distance < distance; }
  );
  return iter != std::end(distances);
}

void game::tick(const delta_t& dt)
{
  for (const auto& action: m_actions)
  {
    if (action.get_type() == control_action_type::press_attack)
    {
      // TODO
    }
    else if (action.get_type() == control_action_type::press_down)
    {
      auto& pos{get_keyboard_player_pos()};
      pos = get_below(pos);
    }
    else if (action.get_type() == control_action_type::press_left)
    {
      auto& pos{get_keyboard_player_pos()};
      pos = get_left(pos);
    }
    else if (action.get_type() == control_action_type::press_move)
    {
      start_move_unit(
        get_keyboard_player_pos(),
        get_keyboard_user_player_color(m_options)
      );
    }
    else if (action.get_type() == control_action_type::press_right)
    {
      auto& pos{get_keyboard_player_pos()};
      pos = get_right(pos);
    }
    else if (action.get_type() == control_action_type::press_select)
    {
      do_select(
        get_keyboard_player_pos(),
        get_keyboard_user_player_color(m_options)
      );
    }
    else if (action.get_type() == control_action_type::press_up)
    {
      auto& pos{get_keyboard_player_pos()};
      pos = get_above(pos);
    }
    else if (action.get_type() == control_action_type::mouse_move)
    {
      auto& pos{get_mouse_player_pos()};
      pos = action.get_coordinat();
    }
    else if (action.get_type() == control_action_type::lmb_down)
    {
      do_select(
        action.get_coordinat(),
        get_mouse_user_player_color(m_options)
      );
    }
    else if (action.get_type() == control_action_type::rmb_down)
    {
      start_move_unit(
        action.get_coordinat(),
        get_mouse_user_player_color(m_options)
      );
    }
  }
  m_actions = std::vector<control_action>();

  for (auto& p: m_pieces) p.tick(dt);

  assert(get_actions().empty());
}

void unselect_all_pieces(
  game & g,
  const chess_color color
)
{
  for (auto& piece: g.get_pieces())
  {
    if (piece.get_color() == color) unselect(piece);
  }
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
    assert(count_game_actions(g) == 0);
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
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 0);
    g.add_action(create_press_rmb_action(black_king.get_coordinat() + game_coordinat(-1.0, 1.0)));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 1);
  }
  // 2x LMB then RMB makes a unit move 1 stretch (not 2)
  {
    game g;
    g.add_action(create_press_lmb_action(get_coordinat("e8")));
    g.add_action(create_press_rmb_action(get_coordinat("e7")));
    g.tick(delta_t(0.001));
    assert(count_piece_actions(g, chess_color::black) == 1);
    g.add_action(create_press_lmb_action(get_coordinat("e8")));
    g.add_action(create_press_rmb_action(get_coordinat("e7")));
    g.tick(delta_t(0.001));
    assert(count_piece_actions(g, chess_color::black) == 1);
  }
  // clear_sound_effects
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(g.get_sound_effects().empty());
    // Add a sound by selecting a piece
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(!g.get_sound_effects().empty());
    g.clear_sound_effects();
    assert(g.get_sound_effects().empty());
  }
  // get_cursor_pos
  {
    const game g;
    assert(get_cursor_pos(g, chess_color::white) != get_cursor_pos(g, chess_color::black));
  }
  // get_keyboard_player_pos and get_mouse_player_pos
  {
    const game g;
    assert(get_keyboard_player_pos(g) != get_mouse_player_pos(g));
  }
  // get_keyboard_player_pos
  {
    game g;
    const auto& pos_before{get_keyboard_player_pos(g)};
    auto& pos = g.get_keyboard_player_pos();
    pos += game_coordinat(0.1, 0.1);
    const auto& pos_after{get_keyboard_player_pos(g)};
    assert(pos_before != pos_after);
  }
  // get_mouse_player_pos
  {
    game g;
    const auto& pos_before{get_mouse_player_pos(g)};
    auto& pos = g.get_mouse_player_pos();
    pos += game_coordinat(0.1, 0.1);
    const auto& pos_after{get_mouse_player_pos(g)};
    assert(pos_before != pos_after);
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
    g.get_keyboard_player_pos() = get_coordinat("e2");
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_select_action());
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(g.get_sound_effects().at(0).get_sound_effect_type() == sound_effect_type::select);
    g.get_keyboard_player_pos() = get_coordinat("e4");
    g.add_action(create_press_move_action());
    g.tick(); // Moves it to e3, unselects piece
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(g.get_closest_piece_to(get_coordinat("e3")).get_type() == piece_type::pawn);
    assert(g.get_sound_effects().at(1).get_sound_effect_type() == sound_effect_type::start_move);
  }
  // Keyboard: cannot move pawn backward
  #ifdef FIX_ISSUE_PAWN_CANNOT_MOVE_BACKWARDS
  {
    game g;
    g.get_keyboard_player_pos() = get_coordinat("e2");
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_select_action());
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(g.get_sound_effects().at(0).get_sound_effect_type() == sound_effect_type::select);
    g.get_keyboard_player_pos() = get_coordinat("e1");
    g.add_action(create_press_move_action());
    g.tick(); // Ignores invalid action, adds sound effect
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(g.get_closest_piece_to(get_coordinat("e2")).get_type() == piece_type::pawn);
    assert(g.get_sound_effects().at(1).get_sound_effect_type() == sound_effect_type::cannot);
  }
  #endif // FIX_ISSUE_PAWN_CANNOT_MOVE_BACKWARDS
#endif // no tests in release
}

void toggle_left_player_color(game& g)
{
  toggle_left_player_color(g.get_options());
}
