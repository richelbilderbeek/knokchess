#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#ifndef LOGIC_ONLY

#include "game.h"
#include "game_resources.h"
#include "game_view_layout.h"
#include <SFML/Graphics.hpp>

/// The game's main window
/// Displays the game class
class game_view
{
public:
  game_view(const game& game = get_default_game());

  /// Run the game, until the user quits
  void exec();

  auto& get_game() noexcept { return m_game; }

  const auto& get_game() const noexcept { return m_game; }

  auto& get_game_resources() noexcept { return m_game_resources; }

  auto& get_window() noexcept { return m_window; }

private:

  /// The game logic
  game m_game;

  /// The resources (images, sounds, etc.) of the game
  game_resources m_game_resources;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the game on-screen
  void show();

  /// Show the mouse cursor on-screen
  void show_mouse_cursor();
};

/// Convert 'true' to 'true' and 'false' to 'false'
std::string bool_to_str(const bool b) noexcept;

/// Create a black/dark square at the right size
sf::RectangleShape create_black_square(game_view& view);

/// Create a white/ligt square at the right size
sf::RectangleShape create_white_square(game_view& view);

/// Show the board: squares, unit paths, pieces, health bars
void show_board(game_view& view);

/// Show the controls (e.g. for a unit) on-screen for player 2
void show_controls_1(game_view& view);

/// Show the controls (e.g. for a unit) on-screen for player 2
void show_controls_2(game_view& view);

/// Show debug info on-screen for player 1
void show_debug_1(game_view& view);

/// Show debug info on-screen for player 2
void show_debug_2(game_view& view);

/// Show the layout of the window: board and panels
void show_layout(game_view& view);

/// Show the pieces of the board on-screen
void show_pieces(game_view& view);

/// Show the info on the side-bar on-screen for player 1
void show_sidebar_1(game_view& view);

/// Show the info on the side-bar on-screen for player 2
void show_sidebar_2(game_view& view);

/// Show the squares of the board on-screen
void show_squares(game_view& view);

/// Show the highlighted square under the cursor on-screen for player 1
void show_square_under_cursor_1(game_view& view);

/// Show the highlighted square under the cursor on-screen for player 2
void show_square_under_cursor_2(game_view& view);

/// Show the pieces' health bars on-screen
void show_unit_health_bars(game_view& view);

/// Show the planned paths for the units on-screen
void show_unit_paths(game_view& view);

/// Show the selected unit(s) on-screen for player 1
void show_unit_sprites_1(game_view& view);

/// Show the selected unit(s) on-screen for player 2
void show_unit_sprites_2(game_view& view);

void test_game_view();

/// Toggle the color of the active player
void toggle_player(game_view& view);

/// Would it be a valid move if the user would press LMB or RMB?
bool would_be_valid(
  const game_view& view,
  chess_color player_color
);

#endif // LOGIC_ONLY

#endif // GAME_VIEW_H
