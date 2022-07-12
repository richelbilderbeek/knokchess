#ifndef GAME_VIEW_LAYOUT_H
#define GAME_VIEW_LAYOUT_H

#include "game_coordinat.h"
#include "screen_coordinat.h"

/// The layout of the game view
///
/// +------------------------------+
/// |                              |
/// | 1-------------+ 2----------+ |
/// | |             | | units    | |
/// | | board       | +----------+ |
/// | |             |              |
/// | |             | +----------+ |
/// | |             | | controls | |
/// | |             | +----------+ |
/// | |             |              |
/// | |             | +----------+ |
/// | |             | | debug    | |
/// | +-------------2 +----------4 |
/// |                              |
/// +------------------------------+
///
/// 1: tl_board
/// 2: br_board
/// 2: tl_side
/// 4: br_side
class game_view_layout
{
public:
  game_view_layout(
    const screen_coordinat& window_size = screen_coordinat(1024, 576),
    const int margin_width = 10
  );

  double get_board_height() const noexcept;
  double get_board_width() const noexcept;

  screen_coordinat get_br_board() const noexcept { return m_br_board; }
  screen_coordinat get_br_side() const noexcept { return m_br_side; }

  double get_square_height() const noexcept;
  double get_square_width() const noexcept;

  screen_coordinat get_tl_board() const noexcept { return m_tl_board; }
  screen_coordinat get_tl_side() const noexcept { return m_tl_side; }

private:
  screen_coordinat m_br_board;
  screen_coordinat m_br_side;
  screen_coordinat m_tl_board;
  screen_coordinat m_tl_side;

  /// The size of the window
  screen_coordinat m_window_size;
};

/// Convert an in-game coordinat to an on-screen coordinat
screen_coordinat convert_to_screen_coordinat(
  const game_coordinat& coordinat,
  const game_view_layout& layout
);

/// Convert an on-screen coordinat to an in-game coordinat
game_coordinat convert_to_game_coordinat(
  const screen_coordinat& coordinat,
  const game_view_layout& layout
);

/// Test the game_view_layout class
void test_game_view_layout();

#endif // GAME_VIEW_LAYOUT_H
