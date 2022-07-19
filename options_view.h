#ifndef OPTIONS_VIEW_H
#define OPTIONS_VIEW_H

#ifndef LOGIC_ONLY

#include "game_options.h"
#include "options_view_layout.h"
#include "game_resources.h"

#include <SFML/Graphics.hpp>

class options_view
{
public:
  options_view(const game_options& options);

  /// Run the menu, until the user quits
  void exec();

  const auto& get_layout() const noexcept { return m_layout; }

  const auto& get_options() const noexcept { return m_options; }

  auto& get_resources() noexcept { return m_resources; }

  const auto& get_window() const noexcept { return m_window; }

  auto& get_window() noexcept { return m_window; }

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  options_view_layout m_layout;

  /// The game options
  game_options m_options;

  game_resources m_resources;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};


/// Show the bottom part
void show_bottom(options_view& v);

/// Show the game speed label and value
void show_game_speed(options_view& v);

/// Show the game speed label and value
void show_music_volume(options_view& v);

/// Show all the layout panels
void show_panels(options_view& v);

void show_starting_position(options_view& v);

/// Show the top part
void show_top(options_view& v);

#endif // LOGIC_ONLY

#endif // OPTIONS_VIEW_H
