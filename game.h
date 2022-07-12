#ifndef GAME_H
#define GAME_H

#include "action.h"
#include "game_coordinat.h"
#include "game_view_layout.h"
#include "piece.h"
#include <vector>

/// Contains the game logic.
/// All data types used by this class are STL and/or Boost
class game
{
public:
  game(
    const screen_coordinat& screen_size = get_default_screen_size(),
    const int margin_width = 10
  );

  /// Add an action. These will be processed in 'tick'
  void add_action(const action a);

  /// Get the piece that is closest to the coordinat
  const piece& get_closest_piece_to(const game_coordinat& coordinat) const;

  /// Get the piece that is closest to the coordinat
  piece& get_closest_piece_to(const game_coordinat& coordinat);

  /// Get the layout of the screen
  const auto& get_layout() const noexcept { return m_layout; }

  /// Get the in-game mouse position
  const auto& get_mouse_pos() const noexcept { return m_mouse_pos; }

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

  /// Go to the next frame
  void tick();

private:

  std::vector<action> m_actions;

  /// The layout of the screen, e.g. the top-left of the sidebar
  game_view_layout m_layout;

  /// The in-game coordinat of the mouse
  game_coordinat m_mouse_pos;

  std::vector<piece> m_pieces;


  /// Process a left-mouse-button down
  void do_lmb_down(const game_coordinat& coordinat);
};

/// Calculate the distances that each piece has to a coordinat
std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
);

game get_default_game() noexcept;

/// Get the index of the piece that is closest to the coordinat
int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinat& coordinat
);

/// Get all the selected pieces
std::vector<piece> get_selected_pieces(const game& g);

/// Get all the pieces
const std::vector<piece>& get_pieces(const game& g) noexcept;

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const game& g,
  const game_coordinat& coordinat,
  const double distance = 0.5
);

void test_game();

#endif // GAME_H
