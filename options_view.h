#ifndef OPTIONS_VIEW_H
#define OPTIONS_VIEW_H

#ifndef LOGIC_ONLY

#include "options_view_layout.h"
#include "game_resources.h"

#include <SFML/Graphics.hpp>

class options_view
{
public:
  options_view();

  /// Run the menu, until the user quits
  void exec();

  const auto& get_layout() const noexcept { return m_layout; }

  auto& get_resources() noexcept { return m_resources; }

  const auto& get_window() const noexcept { return m_window; }

  auto& get_window() noexcept { return m_window; }

private:

  options_view_layout m_layout;

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

/// Show all the layout panels
void show_panels(options_view& v);

/// Show the top part
void show_top(options_view& v);

#endif // LOGIC_ONLY

#endif // OPTIONS_VIEW_H
