#include "controls_view_layout.h"

#include <cassert>
#include <cmath>

controls_view_layout::controls_view_layout(
  const screen_coordinat& window_size,
  const int margin_width
) : m_window_size{window_size}
{
  const int panel_height{
    static_cast<int>(
      static_cast<double>(window_size.get_y() - (9 * margin_width)) / 8.0
    )
  };
  const int panel_width{500};

  const int x1{(m_window_size.get_x() - (2 * panel_width)) / 2};
  const int x2{m_window_size.get_x() - x1};

  const int y1{margin_width};
  const int y2{y1 + panel_height + margin_width + panel_height};
  const int y3{y2 + margin_width};
  const int y4{y3 + panel_height};
  const int y5{y4 + margin_width + panel_height + margin_width};
  const int y6{y5 + panel_height};
  const int y7{y6 + margin_width};
  const int y8{y7 + panel_height};
  const int y9{y8 + margin_width};
  const int y10{y9 + panel_height};
  const int y11{y10 + margin_width};
  const int y12{y11 + panel_height};

  m_title = screen_rect(
    screen_coordinat(x1, y1),
    screen_coordinat(x2, y2)
  );
  m_subtitle = screen_rect(
    screen_coordinat(x1, y3),
    screen_coordinat(x2, y4)
  );
  m_start = screen_rect(
    screen_coordinat(x1, y5),
    screen_coordinat(x2, y6)
  );
  m_options = screen_rect(
    screen_coordinat(x1, y7),
    screen_coordinat(x2, y8)
  );
  m_about = screen_rect(
    screen_coordinat(x1, y9),
    screen_coordinat(x2, y10)
  );
  m_quit = screen_rect(
    screen_coordinat(x1, y11),
    screen_coordinat(x2, y12)
  );
}

std::vector<screen_rect> get_panels(const controls_view_layout& layout)
{
  return
  {
    layout.get_title(),
    layout.get_subtitle(),
    layout.get_start(),
    layout.get_options(),
    layout.get_about(),
    layout.get_quit()
  };
}


const screen_rect& controls_view_layout::get_selectable_rect(const controls_view_item item) const noexcept
{
  switch (item)
  {
    case controls_view_item::start: return m_start;
    case controls_view_item::options: return m_options;
    case controls_view_item::about: return m_about;
    default:
    case controls_view_item::quit:
      assert(item == controls_view_item::quit);
      return m_quit;
  }
}

void test_controls_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const controls_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  // get_selectable_rect
  {
    const controls_view_layout layout;
    assert(layout.get_selectable_rect(controls_view_item::start) == layout.get_start());
    assert(layout.get_selectable_rect(controls_view_item::options) == layout.get_options());
    assert(layout.get_selectable_rect(controls_view_item::about) == layout.get_about());
    assert(layout.get_selectable_rect(controls_view_item::quit) == layout.get_quit());
  }
  #endif
}
