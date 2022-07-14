#include "game_view_layout.h"

#include "helper.h"

#include <cassert>
#include <cmath>

game_view_layout::game_view_layout(
  const screen_coordinat& window_size,
  const int margin_width
) : m_window_size{window_size}
{
  // board
  m_tl_board = screen_coordinat(margin_width, margin_width);
  const int max_board_height = m_window_size.get_y() - margin_width - margin_width;
  const int max_board_width = m_window_size.get_x() - margin_width - margin_width;
  m_board_width = std::min(max_board_height, max_board_width);
  m_board_height = m_board_width;
  m_br_board = screen_coordinat(margin_width + m_board_width, margin_width + m_board_height);

  // side
  m_panel_height = static_cast<int>(static_cast<double>(window_size.get_y() - (4 * margin_width)) / 3.0);
  m_panel_width = static_cast<int>(static_cast<double>(window_size.get_x() - m_board_width - (3 * margin_width)));
  m_tl_units_2 = screen_coordinat(margin_width + m_board_width + margin_width, margin_width);
  m_tl_controls_2 = m_tl_units_2 + screen_coordinat(0, m_panel_height + margin_width);
  m_tl_debug_2 = m_tl_controls_2 + screen_coordinat(0, m_panel_height + margin_width);


  m_br_units_2 = m_tl_units_2 + screen_coordinat(m_panel_width, m_panel_height);
  m_br_controls_2 = m_tl_controls_2 + screen_coordinat(m_panel_width, m_panel_height);
  m_br_debug_2 = m_tl_debug_2 + screen_coordinat(m_panel_width, m_panel_height);
}

game_coordinat convert_to_game_coordinat(
  const screen_coordinat& coordinat,
  const game_view_layout& layout
)
{
  // How many pixels on the board
  const int screen_on_board_x{
    coordinat.get_x() - layout.get_tl_board().get_x()
  };
  const int screen_on_board_y{
    coordinat.get_y() - layout.get_tl_board().get_y()
  };
  // Fraction of the board
  const double f_x{
    static_cast<double>(screen_on_board_x) / static_cast<double>(layout.get_board_width())
  };
  const double f_y{
    static_cast<double>(screen_on_board_y) / static_cast<double>(layout.get_board_height())
  };
  return game_coordinat(
    8.0 * f_x,
    8.0 - (8.0 * f_y)
  );
}

screen_coordinat convert_to_screen_coordinat(
  const game_coordinat& coordinat,
  const game_view_layout& layout
)
{
  const auto tl_board{layout.get_tl_board()};
  const auto br_board{layout.get_br_board()};
  const double square_width{
    static_cast<double>(br_board.get_x() - tl_board.get_x()) / 8.0
  };
  const double square_height{
    static_cast<double>(br_board.get_y() - tl_board.get_y()) / 8.0
  };
  return screen_coordinat(
    tl_board.get_x() + (square_width * coordinat.get_x()),
    br_board.get_y() - (square_height * coordinat.get_y()) // The first rank (e.g. with the white king) is at the bottom
  );
}

double game_view_layout::get_square_height() const noexcept
{
  return static_cast<double>(get_board_height()) / 8.0;
}

double game_view_layout::get_square_width() const noexcept
{
  return static_cast<double>(get_board_width()) / 8.0;
}

void test_game_view_layout()
{
  #ifndef NDEBUG
  // Minimal tests
  {
    const game_view_layout layout;

    assert(layout.get_br_board().get_x() > 0.0);
    assert(layout.get_br_board().get_y() > 0.0);
    assert(layout.get_tl_board().get_x() > 0.0);
    assert(layout.get_tl_board().get_y() > 0.0);

    assert(layout.get_br_debug_2().get_x() > 0.0);
    assert(layout.get_br_debug_2().get_y() > 0.0);
    assert(layout.get_tl_units_2().get_x() > 0.0);
    assert(layout.get_tl_units_2().get_y() > 0.0);
    assert(layout.get_board_width() > 0.0);
    assert(layout.get_board_height() > 0.0);
    assert(layout.get_square_width() > 0.0);
    assert(layout.get_square_height() > 0.0);
  }
  // Coordinats of default size
  {
    const int margin_width = 10;
    const game_view_layout layout{screen_coordinat(1024, 576), margin_width};
    assert(layout.get_tl_board().get_x() == 0.0 + margin_width);
    assert(layout.get_tl_board().get_y() == 0.0 + margin_width);
    assert(layout.get_br_board().get_x() == 576.0 - margin_width);
    assert(layout.get_br_board().get_y() == 576.0 - margin_width);

    assert(layout.get_tl_units_2().get_x() == 576.0 - margin_width + margin_width);
    assert(layout.get_tl_units_2().get_y() == margin_width);
    assert(layout.get_br_debug_2().get_x() == 1024 - margin_width);
    assert(is_close(layout.get_br_debug_2().get_y(), 576 - margin_width, 3));
    const int board_width{layout.get_board_width()};
    assert(board_width == 576.0 - margin_width - margin_width);
    const int board_height{layout.get_board_height()};
    assert(board_height == 576.0 - margin_width - margin_width);
    const double square_width{board_width / 8.0};
    assert(layout.get_square_width() == square_width);
    const double square_height{board_height / 8.0};
    assert(layout.get_square_height() == square_height);
  }
  //--------------------------------------------------------------------------
  // game -> screen
  //--------------------------------------------------------------------------
  // in-game (0,8) must be top-left of screen board
  {
    const game_view_layout layout;
    const auto tl_board = convert_to_screen_coordinat(
      game_coordinat(0.0, 8.0),
      layout
    );
    assert(tl_board.get_x() == layout.get_tl_board().get_x());
    assert(tl_board.get_y() == layout.get_tl_board().get_y());
  }
  // in-game (8,0) must be bottom-right of screen board
  // (no piece can ever have its top-right at the bottom-right of the board)
  {
    const game_view_layout layout;
    const auto br_board = convert_to_screen_coordinat(
      game_coordinat(8.0, 0.0),
      layout
    );
    assert(br_board.get_x() == layout.get_br_board().get_x());
    assert(br_board.get_y() == layout.get_br_board().get_y());
  }
  //--------------------------------------------------------------------------
  // screen -> game
  //--------------------------------------------------------------------------
  // top-left of screen board must be in-game (0,8)
  // bottom-right of screen board must be in-game (8,0)
  {
    const game_view_layout layout;
    const auto br_board = convert_to_game_coordinat(
      layout.get_br_board(),
      layout
    );
    const auto tl_board = convert_to_game_coordinat(
      layout.get_tl_board(),
      layout
    );
    assert(tl_board.get_x() == 0.0);
    assert(tl_board.get_y() == 8.0);
    assert(br_board.get_x() == 8.0);
    assert(br_board.get_y() == 0.0);
  }
  #endif
}
