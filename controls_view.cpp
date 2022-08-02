#include "controls_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinat.h"
#include "game_view.h"
#include "options_view.h"

#include "sfml_helper.h"
#include <cassert>
#include <cmath>
#include <iostream>

controls_view::controls_view(controller c)
  : m_controller(c),
    m_selected{controls_view_item::type}
{

}

void controls_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_controls_screen_size().get_x(),
      get_default_controls_screen_size().get_y()
    ),
    "Conquer Chess: controls menu"
  );

  // Center
  auto desktop = sf::VideoMode::getDesktopMode();
  m_window.setPosition(
    sf::Vector2i(
      (desktop.width / 2) - (m_window.getSize().x /2),
      (desktop.height/ 2) - (m_window.getSize().y /2)
    )
  );

  while (m_window.isOpen())
  {
    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Show the new state
    show();

  }
}

bool controls_view::process_events()
{
  // User interaction
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    if (event.type == sf::Event::Resized)
    {
      // From https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
      const sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
      m_window.setView(sf::View(visible_area));
      m_layout = controls_view_layout(
        screen_coordinat(event.size.width, event.size.height),
        get_default_margin_width()
      );
    }
    else if (event.type == sf::Event::Closed)
    {
      m_window.close();
      return true; // Menu is done
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
        m_selected = get_previous(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Right)
      {
        m_selected = get_next(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Down)
      {
        m_selected = get_next(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Left)
      {
        m_selected = get_previous(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Space)
      {
        /*
        if (m_selected == controls_view_item::start) exec_game();
        else if (m_selected == controls_view_item::options) exec_options();
        else if (m_selected == controls_view_item::quit)
        {
          m_window.close();
          return true;
        }
        */
      }
      else if (key_pressed == sf::Keyboard::Key::Q)
      {
        m_window.close();
        return true;
      }
      else if (key_pressed == sf::Keyboard::Key::F3)
      {
        // debug
        std::clog << "Debug";
      }
    }
    /*
    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        const auto mouse_screen_pos{
          screen_coordinat(event.mouseButton.x, event.mouseButton.y)
        };
        if (is_in(mouse_screen_pos, m_layout.get_start())) exec_game();
        else if (is_in(mouse_screen_pos, m_layout.get_options())) exec_options();
        else if (is_in(mouse_screen_pos, m_layout.get_quit()))
        {
          m_window.close();
          return true;
        }
      }
    }
    */
  }
  return false; // Do not close the window :-)
}

void controls_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  show_panels(*this);

  //show_title_panel(*this);

  // Display all shapes
  m_window.display();

}

void show_panels(controls_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    v.get_window().draw(rectangle);
  }
}

#endif // LOGIC_ONLY
