#include "options_view.h"

#ifndef LOGIC_ONLY

#include "options_view.h"

#include "screen_coordinat.h"
#include "game_view.h"
#include "sfml_helper.h"
#include <iostream>

options_view::options_view()
{

}

void options_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_screen_size().get_x(),
      get_default_screen_size().get_y()
    ),
    "Conquer Chess: options menu"
  );
  while (m_window.isOpen())
  {
    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Show the current options on-screen
    show();
  }
}

bool options_view::process_events()
{
  // User interaction
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    if (event.type == sf::Event::Resized)
    {
      // From https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
      const sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
      m_window.setView(sf::View(visibleArea));
      m_layout = options_view_layout(
        screen_coordinat(visibleArea.width, visibleArea.height),
        get_default_margin_width()
      );
    }
    else if (event.type == sf::Event::Closed)
    {
        m_window.close();
        return true; // Game is done
    }
    else if (event.type == sf::Event::KeyPressed)
    {
      sf::Keyboard::Key key_pressed = event.key.code;
      if (key_pressed == sf::Keyboard::Key::Escape)
      {
        m_window.close();
        return true;
      }
      else if (key_pressed == sf::Keyboard::Key::Up)
      {
        //m_game.add_action(create_press_up_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Right)
      {
        //m_game.add_action(create_press_right_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Down)
      {
        //m_game.add_action(create_press_down_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Left)
      {
        //m_game.add_action(create_press_left_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Space)
      {
        //m_game.add_action(create_press_select_action());
      }
      else if (key_pressed == sf::Keyboard::Key::M)
      {
        //m_game.add_action(create_press_move_action());
      }
      else if (key_pressed == sf::Keyboard::Key::A)
      {
        //m_game.add_action(create_press_attack_action());
      }
      else if (key_pressed == sf::Keyboard::Key::F3)
      {
        // debug
        std::clog << "Debug";
      }
    }
  }
  return false; // if no events proceed with tick
}

void options_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  show_panels(*this);

  show_top(*this);
  show_bottom(*this);

  // Display all shapes
  m_window.display();

}

void show_top(options_view& v)
{
  const auto& layout = v.get_layout();
  // game speed label
  {
    const auto& screen_rect = layout.get_game_speed_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(132, 32, 32));
    v.get_window().draw(rectangle);
  }
  // game speed value
  {
    const auto& screen_rect = layout.get_game_speed_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 132, 32));
    v.get_window().draw(rectangle);
  }
  // music volume label
  {
    const auto& screen_rect = layout.get_music_volume_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 132));
    v.get_window().draw(rectangle);
  }
  // music volume value
  {
    const auto& screen_rect = layout.get_music_volume_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(232, 32, 32));
    v.get_window().draw(rectangle);
  }
  // starting pos label
  {
    const auto& screen_rect = layout.get_starting_pos_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 232, 32));
    v.get_window().draw(rectangle);
  }
  // starting pos value
  {
    const auto& screen_rect = layout.get_starting_pos_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 232));
    v.get_window().draw(rectangle);
  }
}


void show_panels(options_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    v.get_window().draw(rectangle);
  }
}

void show_bottom(options_view& v)
{
  const auto& layout = v.get_layout();
  {
    const auto& screen_rect = layout.get_player_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    v.get_window().draw(rectangle);
  }
  {
    const auto& screen_rect = layout.get_color_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(132, 32, 32));
    v.get_window().draw(rectangle);
  }
  {
    const auto& screen_rect = layout.get_controls_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 132, 32));
    v.get_window().draw(rectangle);
  }
  {
    const auto& screen_rect = layout.get_left_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 132));
    v.get_window().draw(rectangle);
  }
  {
    const auto& screen_rect = layout.get_right_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(232, 32, 32));
    v.get_window().draw(rectangle);
  }

  {
    const auto& screen_rect = layout.get_left_color_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 232, 32));
    v.get_window().draw(rectangle);
  }
  {
    const auto& screen_rect = layout.get_right_color_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 232));
    v.get_window().draw(rectangle);
  }
  {
    const auto& screen_rect = layout.get_left_controls_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(232, 232, 232));
    v.get_window().draw(rectangle);
  }
  {
    const auto& screen_rect = layout.get_right_controls_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(132, 132, 123));
    v.get_window().draw(rectangle);
  }
}

#endif // LOGIC_ONLY
