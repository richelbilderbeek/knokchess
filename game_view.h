#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "game.h"
#include "game_resources.h"
#include "game_view_layout.h"
#include <SFML/Graphics.hpp>

/// The game's main window
/// Displays the game class
class game_view
{
public:
  game_view(const game& game);

  /// Run the game, until the user quits
  void exec();

  std::pair<double, double> convert_screen_to_board(const int x, const int y);

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

  /// Show the pieces' health bars on-screen
  void show_health_bars();

  /// Show the mouse cursor on-screen
  void show_mouse_cursor();

  /// Show the pieces of the board on-screen
  void show_pieces();

  /// Show the info on the side-bar on-screen
  void show_sidebar();

  /// Show the squares of the board on-screen
  void show_squares();
};

void test_game_view();


#endif // GAME_VIEW_H