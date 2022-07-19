#include "game_options.h"

#include "game_view_layout.h"
#include <cassert>

game_options::game_options(
  const screen_coordinat& screen_size,
  const starting_position_type starting_position,
  const delta_t& dt,
  const int margin_width
) : m_delta_t{dt},
    m_left_controller_type{controller_type::keyboard},
    m_left_player_color{chess_color::white},
    m_margin_width{margin_width},
    m_right_controller_type{controller_type::mouse},
    m_screen_size{screen_size},
    m_starting_position{starting_position},
    m_volume{0.0}
{
  assert(m_margin_width >= 0);
  assert(m_screen_size.get_x() > 0);
  assert(m_screen_size.get_y() > 0);
  assert(m_volume >= 0.0);
  assert(m_volume <= 100.0);
  assert(::get_keyboard_user_player_color(*this) != ::get_mouse_user_player_color(*this));
}

bool do_show_selected(const game_options& options) noexcept
{
  return options.do_show_selected();
}

game_options get_default_game_options()
{
  return game_options(
    get_default_screen_size(),
    get_default_starting_position(),
    get_default_delta_t(),
    get_default_margin_width()
  );
}

double get_default_delta_t()
{
  return 0.0001;
}

chess_color get_keyboard_user_player_color(const game_options& options)
{
  if (options.get_left_controller_type() == controller_type::keyboard)
  {
    return get_left_player_color(options);
  }
  assert(options.get_right_controller_type() == controller_type::keyboard);
  return get_right_player_color(options);
}

chess_color get_left_player_color(const game_options& options) noexcept
{
  return options.get_left_player_color();
}

chess_color get_mouse_user_player_color(const game_options& options)
{
  if (options.get_left_controller_type() == controller_type::mouse)
  {
    return get_left_player_color(options);
  }
  assert(options.get_right_controller_type() == controller_type::mouse);
  return get_right_player_color(options);
}

chess_color get_right_player_color(const game_options& options) noexcept
{
  return get_other_color(options.get_left_player_color());
}

void game_options::set_left_player_color(const chess_color c) noexcept
{
  m_left_player_color = c;
}

void toggle_player(game_options& options)
{
  options.set_left_player_color(
    get_other_color(options.get_left_player_color())
  );
}
