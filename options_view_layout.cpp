#include "options_view_layout.h"

#include "options_view_layout.h"

#include <cassert>
#include <cmath>

options_view_layout::options_view_layout(
  const screen_coordinat& window_size,
  const int margin_width
) : m_window_size{window_size}
{
  const int panel_height{
    static_cast<int>(
      static_cast<double>(window_size.get_y() - (3 * margin_width)) / 6.0
    )
  };
  const int top_panel_width{
    static_cast<int>(
      static_cast<double>(window_size.get_x() - (2 * margin_width)) / 2.0
    )
  };
  const int bottom_panel_width{
    static_cast<int>(
      static_cast<double>(window_size.get_x() - (2 * margin_width)) / 3.0
    )
  };

  const int x1{margin_width};
  const int x2{x1 + bottom_panel_width};
  const int x3{x1 + top_panel_width};
  const int x4{x2 + bottom_panel_width};
  const int x5{x3 + top_panel_width};

  const int y1{margin_width};
  const int y2{y1 + panel_height};
  const int y3{y2 + panel_height};
  const int y4{y3 + panel_height};
  const int y5{y4 + margin_width};
  const int y6{y5 + panel_height};
  const int y7{y6 + panel_height};
  const int y8{y7 + panel_height};

  m_game_speed_label = screen_rect(
    screen_coordinat(x1, y1),
    screen_coordinat(x3, y2)
  );
  m_game_speed_value = screen_rect(
    screen_coordinat(x3, y1),
    screen_coordinat(x5, y2)
  );

  m_music_volume_label = screen_rect(
    screen_coordinat(x1, y2),
    screen_coordinat(x3, y3)
  );
  m_music_volume_value = screen_rect(
    screen_coordinat(x3, y2),
    screen_coordinat(x5, y3)
  );

  m_starting_pos_label = screen_rect(
    screen_coordinat(x1, y3),
    screen_coordinat(x3, y4)
  );
  m_starting_pos_value = screen_rect(
    screen_coordinat(x3, y3),
    screen_coordinat(x5, y4)
  );

  m_player_label = screen_rect(
    screen_coordinat(x1, y5),
    screen_coordinat(x2, y6)
  );
  m_color_label = screen_rect(
    screen_coordinat(x2, y5),
    screen_coordinat(x4, y6)
  );
  m_controls_label = screen_rect(
    screen_coordinat(x4, y5),
    screen_coordinat(x5, y6)
  );
  m_left_label = screen_rect(
    screen_coordinat(x1, y6),
    screen_coordinat(x2, y7)
  );
  m_right_label = screen_rect(
    screen_coordinat(x1, y7),
    screen_coordinat(x2, y8)
  );

  m_left_color_value = screen_rect(
    screen_coordinat(x2, y6),
    screen_coordinat(x4, y7)
  );
  m_right_color_value = screen_rect(
    screen_coordinat(x2, y7),
    screen_coordinat(x4, y8)
  );

  m_left_controls_value = screen_rect(
    screen_coordinat(x4, y6),
    screen_coordinat(x5, y7)
  );
  m_right_controls_value = screen_rect(
    screen_coordinat(x4, y7),
    screen_coordinat(x5, y8)
  );
}

const screen_rect& options_view_layout::get_selectable_rect(const options_view_item item) const noexcept
{
  switch (item)
  {
    case options_view_item::game_speed: return m_game_speed_value;
    case options_view_item::music_volume: return m_music_volume_value;
    case options_view_item::starting_position: return m_starting_pos_value;
    case options_view_item::left_color: return m_left_color_value;
    case options_view_item::right_color: return m_right_color_value;
    case options_view_item::left_controls: return m_left_controls_value;
    default:
    case options_view_item::right_controls:
      assert(item == options_view_item::right_controls);
      return m_right_controls_value;
  }
}

std::vector<screen_rect> get_panels(const options_view_layout& layout)
{
  return
  {
    layout.get_game_speed_label(),
    layout.get_game_speed_value(),
    layout.get_music_volume_label(),
    layout.get_music_volume_value(),
    layout.get_starting_pos_label(),
    layout.get_starting_pos_value(),
    layout.get_player_label(),
    layout.get_color_label(),
    layout.get_controls_label(),
    layout.get_left_label(),
    layout.get_right_label(),
    layout.get_left_color_value(),
    layout.get_right_color_value(),
    layout.get_left_controls_value(),
    layout.get_right_controls_value()
  };
}

void test_options_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const options_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  // get_selectable_rect
  {
    const options_view_layout layout;
    assert(layout.get_selectable_rect(options_view_item::game_speed) == layout.get_game_speed_value());
    assert(layout.get_selectable_rect(options_view_item::music_volume) == layout.get_music_volume_value());
    assert(layout.get_selectable_rect(options_view_item::starting_position) == layout.get_starting_pos_value());
    assert(layout.get_selectable_rect(options_view_item::left_color) == layout.get_left_color_value());
    assert(layout.get_selectable_rect(options_view_item::left_controls) == layout.get_left_controls_value());
    assert(layout.get_selectable_rect(options_view_item::right_color) == layout.get_right_color_value());
    assert(layout.get_selectable_rect(options_view_item::right_controls) == layout.get_right_controls_value());
  }
  #endif
}
