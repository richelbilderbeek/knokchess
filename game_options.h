#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include <vector>

#include "controller_type.h"
#include "delta_t.h"
#include "piece.h"
#include "screen_coordinat.h"
#include "starting_position_type.h"
#include "volume.h"
#include "game_speed.h"

/// Options for the game, such as speed
class game_options
{
public:
  game_options(
    const screen_coordinat& screen_size,
    const starting_position_type starting_position,
    const game_speed speed,
    const int margin_width
  );

  /// Are selected units shown on-screen?
  auto do_show_selected() const noexcept { return false; }

  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  auto get_click_distance() const noexcept { return m_click_distance; }

  /// Get the game speed
  auto get_game_speed() const noexcept { return m_game_speed; }

  /// Controller type of the left player
  auto get_left_controller_type() const noexcept { return m_left_controller_type; }

  /// Color of the left player
  auto get_left_player_color() const noexcept { return m_left_player_color; }

  /// Get the width of the margin in pixels
  auto get_margin_width() const noexcept { return m_margin_width; }

  /// Controller type of the left player
  auto get_right_controller_type() const noexcept { return m_right_controller_type; }

  /// Get the size of the screen in pixels
  const auto& get_screen_size() const noexcept { return m_screen_size; }

  /// Get the starting position
  auto get_starting_position() const noexcept { return m_starting_position; }

  /// Get the volume, as a percentage
  auto get_volume() const noexcept { return m_volume; }

  /// Set the game speed
  void set_game_speed(const game_speed speed) noexcept { m_game_speed = speed; }

  /// Set the color of the player
  void set_left_player_color(const chess_color c) noexcept;

  /// Set the controller type for the left player
  void set_left_controller_type(const controller_type t) noexcept;

  /// Set the controller type for the right player
  void set_right_controller_type(const controller_type t) noexcept;

  /// Set the starting position
  void set_starting_position(const starting_position_type starting_position) noexcept { m_starting_position = starting_position; }

  /// Set the volume, as a percentage
  void set_volume(const volume& v) noexcept { m_volume = v; }

private:

  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  double m_click_distance;

  /// The game speed
  game_speed m_game_speed;

  /// Controller type of the left player
  controller_type m_left_controller_type;

  /// Color of the left player
  chess_color m_left_player_color;

  /// The width of the margin in pixels
  int m_margin_width;

  /// Controller type of the right player
  controller_type m_right_controller_type;

  /// The size of the screen in pixels
  screen_coordinat m_screen_size;

  /// The starting position
  starting_position_type m_starting_position;

  /// Volume in percent: 0 is silent, 100 is loudest
  volume m_volume;
};

/// Are selected squares shown on-screen?
bool do_show_selected(const game_options& options) noexcept;

game_options get_default_game_options();

/// Get the color of the keyboard using player
/// Will throw if no user uses a keyboard
chess_color get_keyboard_user_player_color(const game_options& options);

/// Get the color of the left player
chess_color get_left_player_color(const game_options& options) noexcept;

/// Get the controller of the left player
controller_type get_left_player_controller(const game_options& options) noexcept;

/// Get the color of the mouse using player
/// Will throw if no user uses a mouse
chess_color get_mouse_user_player_color(const game_options& options);

/// Get the color of the right player
chess_color get_right_player_color(const game_options& options) noexcept;

/// Get the controller of the right player
controller_type get_right_player_controller(const game_options& options) noexcept;

/// Get all the pieces in the starting position type
std::vector<piece> get_starting_pieces(
  const game_options& options
) noexcept;

/// Test this class and its free functions
void test_game_options();

/// Toggle the color of the active player
void toggle_left_player_color(game_options& options);

#endif // GAME_OPTIONS_H
