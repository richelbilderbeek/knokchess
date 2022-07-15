#ifndef GAME_H
#define GAME_H

#include "action.h"
#include "game_coordinat.h"
#include "game_options.h"
#include "game_view_layout.h"
#include "piece.h"
#include <vector>

/// Contains the game logic.
/// All data types used by this class are STL and/or Boost
class game
{
public:
  game(
    const game_options& options = get_default_game_options()
  );

  /// Add an action. These will be processed in 'tick'
  void add_action(const action a);

  /// Get the game actions, should be zero after each tick
  const auto& get_actions() const noexcept { return m_actions; }

  /// Get the piece that is closest to the coordinat
  const piece& get_closest_piece_to(const game_coordinat& coordinat) const;

  /// Get the piece that is closest to the coordinat
  piece& get_closest_piece_to(const game_coordinat& coordinat);

  /// Get the layout of the screen
  const auto& get_layout() const noexcept { return m_layout; }

  /// Get the in-game keyboard position
  const auto& get_player_1_pos() const noexcept { return m_player_1_pos; }

  /// Get the in-game mouse position
  const auto& get_player_2_pos() const noexcept { return m_player_2_pos; }

  /// Get the game options
  auto& get_options() noexcept { return m_options; }

  /// Get the game options
  const auto& get_options() const noexcept { return m_options; }

  /// Get all the pieces
  auto& get_pieces() noexcept { return m_pieces; }

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

  /// Go to the next frame
  void tick();

private:

  std::vector<action> m_actions;

  /// The layout of the screen, e.g. the top-left of the sidebar
  game_view_layout m_layout;

  /// The in-game coordinat of the keyboard user
  game_coordinat m_player_1_pos;

  /// The in-game coordinat of the mouse
  game_coordinat m_player_2_pos;

  /// The game options
  game_options m_options;

  std::vector<piece> m_pieces;

  /// Process a space or left-mouse-button
  void do_select(
    const game_coordinat& coordinat,
    const chess_color player_color
  );

  /// Process an M or right-mouse-button down
  void start_move_unit(
    const game_coordinat& coordinat,
    const chess_color player_color
  );
};

/// Calculate the distances that each piece has to a coordinat
std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
);

/// Can the player select a piece at the current mouse position?
bool can_select_piece_at_mouse_pos(const game& g);

/// Count the total number of actions to be done by the game,
/// which should be zero after each tick
int count_game_actions(const game& g);

/// Count the total number of actions to be done by pieces
int count_piece_actions(const game& g);

/// Count the number of selected units
int count_selected_units(const game& g);

/// Create a game in which it is only a king versus a king,
/// to be used in debugging
game create_king_versus_king_game();

/// Find zero, one or more chess pieces of the specified type and color
std::vector<piece> find_pieces(
  const game& g,
  const piece_type type,
  const chess_color color
);

game get_default_game() noexcept;

/// Get the index of the piece that is closest to the coordinat
int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinat& coordinat
);

/// Get all the selected pieces
/// @param g a game
/// @param player the color of the player, which is white for player 1
/// @see use 'has_selected_piece' to see if there is at least 1 piece selected
std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
);

/// Get all the pieces
const std::vector<piece>& get_pieces(const game& g) noexcept;

/// See if there is at least 1 piece selected
/// @param g a game
/// @param player the color of the player, which is white for player 1
/// @see use 'get_selected_pieces' to get all the selected pieces
bool has_selected_pieces(const game& g, const chess_color player);

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const game& g,
  const game_coordinat& coordinat,
  const double distance = 0.5
);

void test_game();

/// Toggle the color of the active player
void toggle_player(game& g);

/// Unselect all pieces of a certain color
void unselect_all_pieces(
  game & g,
  const chess_color color
);

#endif // GAME_H
