#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#ifndef LOGIC_ONLY

#include "ccfwd.h"
#include "game.h"
#include "fps_clock.h"
#include "game_log.h"
#include "game_resources.h"
#include "game_view_layout.h"
#include <SFML/Graphics.hpp>

/// The game's main window
/// Displays the game class
class game_view
{
public:
  explicit game_view(const game& game = get_default_game());

  /// Run the game, until the user quits
  void exec();

  /// The the elapsed time in seconds
  double get_elapsed_time_secs() const noexcept;

  int get_fps() const noexcept { return m_fps_clock.get_fps(); }

  auto& get_game() noexcept { return m_game; }

  const auto& get_game() const noexcept { return m_game; }

  auto& get_resources() noexcept { return m_game_resources; }

  /// Get the text log, i.e. things pieces have to say
  const auto& get_log() const noexcept { return m_log; }

  auto& get_window() noexcept { return m_window; }

private:

  /// The game clock, to measure the elapsed time
  sf::Clock m_clock;

  /// The FPS clock
  fps_clock m_fps_clock;

  /// The game logic
  game m_game;

  /// The resources (images, sounds, etc.) of the game
  game_resources m_game_resources;

  /// The text log
  game_log m_log;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// Play the new sound effects
  void play_pieces_sound_effects();

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Read the pieces' messages and play their sounds
  void process_piece_messages();

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

/// Are selected squares shown on-screen?
bool do_show_selected(const game_view& view);

/// Get the controls text for a player with a controller
std::string get_controls_text(
  const game_view& view,
  const chess_color player,
  const controller_type controller,
  const int key
);

/// Get the frames per second
int get_fps(const game_view& v) noexcept;

/// Get the last log messages for a player
std::string get_last_log_messages(
  const game_view& v,
  const side player
) noexcept;

/// Get the layout
const game_view_layout& get_layout(const game_view& v) noexcept;

/// Get the game options
const game_options& get_options(const game_view& v) noexcept;

/// Get the pieces
const std::vector<piece>& get_pieces(const game_view& v) noexcept;

/// Get the time in the game
const delta_t& get_time(const game_view& v) noexcept;

/// Show the board: squares, unit paths, pieces, health bars
void show_board(game_view& view);

/// Show the controls (e.g. for a unit) on-screen for a player
void show_controls(game_view& view, const side player);

/// Show debug info on-screen for player 1
void show_debug_1(game_view& view);

/// Show debug info on-screen for player 2
void show_debug_2(game_view& view);

/// Show the layout of the window: board and panels
void show_layout(game_view& view);

/// Show the log on-screen, i.e. things the pieces say
void show_log(game_view& view, const side player);

/// Show the squares that are occupied on-screen
/// Throws if this option is turned off
void show_occupied_squares(game_view& view);

/// Show the pieces of the board on-screen
void show_pieces(game_view& view);

/// Show the possible moves for the selected pieces.
/// e.g. put crosses on squares that can be attacked.
/// Does nothing if no pieces are selected
void show_possible_moves(game_view& view, const side player);

/// Show the info on the side-bar on-screen for player 1
void show_sidebar_1(game_view& view);

/// Show the info on the side-bar on-screen for player 2
void show_sidebar_2(game_view& view);

/// Show the squares of the board on-screen
void show_squares(game_view& view);

/// Show the highlighted square under the cursor on-screen for a player
void show_square_under_cursor(game_view& view, const side player);

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
void toggle_left_player_color(game_view& view);

/// Would it be a valid move if the user would press LMB or RMB?
bool would_be_valid(
  const game_view& view,
  chess_color player_color
);

#endif // LOGIC_ONLY

#endif // GAME_VIEW_H
