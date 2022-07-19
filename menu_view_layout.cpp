#include "menu_view_layout.h"

#include <cassert>
#include <cmath>

menu_view_layout::menu_view_layout(
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

screen_rect get_next(const screen_rect& there, const menu_view_layout& layout)
{
  if (there == layout.get_start()) return layout.get_options();
  if (there == layout.get_options()) return layout.get_about();
  if (there == layout.get_about()) return layout.get_quit();
  assert(there == layout.get_quit());
  return layout.get_start();
}

std::vector<screen_rect> get_panels(const menu_view_layout& layout)
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

screen_rect get_previous(const screen_rect& there, const menu_view_layout& layout)
{
  if (there == layout.get_start()) return layout.get_quit();
  if (there == layout.get_options()) return layout.get_start();
  if (there == layout.get_about()) return layout.get_options();
  assert(there == layout.get_quit());
  return layout.get_about();
}

void resize(
  menu_view_layout& g,
  const screen_coordinat& window_size,
  const int margin_width
)
{
  g = menu_view_layout(
    window_size,
    margin_width
  );
}

void test_menu_view_layout()
{
  #ifndef NDEBUG
  // Minimal tests
  {
    const menu_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  // Get next
  {
    const menu_view_layout layout;
    const auto start{layout.get_start()};
    const auto options{layout.get_options()};
    const auto about{layout.get_about()};
    const auto quit{layout.get_quit()};
    assert(get_next(start, layout) == options);
    assert(get_next(options, layout) == about);
    assert(get_next(about, layout) == quit);
    assert(get_next(quit, layout) == start);
  }
  // Get previous
  {
    const menu_view_layout layout;
    const auto start{layout.get_start()};
    const auto options{layout.get_options()};
    const auto about{layout.get_about()};
    const auto quit{layout.get_quit()};
    assert(get_previous(start, layout) == quit);
    assert(get_previous(options, layout) == start);
    assert(get_previous(about, layout) == options);
    assert(get_previous(quit, layout) == about);
  }
  #endif
}
