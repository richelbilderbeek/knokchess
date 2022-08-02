#include "controller.h"

#include "game.h"
#include "sfml_helper.h"

#include <cassert>

controller::controller(
  const controller_type type,
  const side player
)
  : m_player{player},
    m_type{type}
{

}

sf::Event create_key_pressed_event(const sf::Keyboard::Key k)
{
  sf::Event e;
  e.type = sf::Event::KeyPressed;
  e.key.code = k;
  return e;
}

std::string get_text_for_action(
  const controller& c,
  const bool has_selected_units,
  const int action_key_number
) noexcept
{
  assert(action_key_number >= 1); // Human based counting
  assert(action_key_number <= 4); // Human based counting
  if (c.get_type() == controller_type::keyboard)
  {
    const std::string key_str{
      to_str(c.get_key_bindings().get_key_for_action(action_key_number))
    };
    if (has_selected_units)
    {
      switch (action_key_number)
      {
        case 1: return key_str + "\nMove";
        case 2: return key_str + "\nAttack";
        case 3: return key_str;
        case 4:
          default:
          assert(action_key_number == 4);
          return key_str;
      }
    }
    else
    {
      switch (action_key_number)
      {
        case 1: return key_str + "\nSelect";
        case 2: return key_str;
        case 3: return key_str;
        case 4:
          default:
          assert(action_key_number== 4);
          return key_str;
      }
    }
  }
  else
  {
    assert(c.get_type() == controller_type::mouse);
    const std::string key_str{std::to_string(action_key_number)};
    if (has_selected_units)
    {
      switch (action_key_number)
      {
        case 1: return key_str + "\nMove";
        case 2: return key_str + "\nAttack";
        case 3: return key_str;
        case 4:
          default:
          assert(action_key_number == 4);
          return key_str;
      }
    }
    else
    {
      switch (action_key_number)
      {
        case 1: return key_str + "\nSelect";
        case 2: return key_str;
        case 3: return key_str;
        case 4:
          default:
          assert(action_key_number== 4);
          return key_str;
      }
    }
  }
}

std::vector<control_action> controller::process_input(
  const sf::Event& event,
  const game& g
) const
{
  if (m_type == controller_type::keyboard)
  {
    if (event.type == sf::Event::KeyPressed)
    {
      return process_key_press(event);
    }
  }
  else
  {
    assert(m_type == controller_type::mouse);
    if (event.type == sf::Event::MouseMoved)
    {
      return process_mouse_moved(event, g);
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
      return process_mouse_pressed(event, g);
    }
  }
  // Ignore the rest
  return {};
}

std::vector<control_action> controller::process_key_press(
  const sf::Event& event
) const
{
  assert(event.type == sf::Event::KeyPressed);

  sf::Keyboard::Key key_pressed = event.key.code;
  if (key_pressed == m_key_bindings.get_key_for_move_up())
  {
    return { create_press_up_action() };
  }
  else if (key_pressed == m_key_bindings.get_key_for_move_right())
  {
    return { create_press_right_action() };
  }
  else if (key_pressed == m_key_bindings.get_key_for_move_down())
  {
    return { create_press_down_action() };
  }
  else if (key_pressed == m_key_bindings.get_key_for_move_left())
  {
    return { create_press_left_action() };
  }
  else if (key_pressed == m_key_bindings.get_key_for_action(1))
  {
    return { create_press_select_action() };
  }
  else if (key_pressed == m_key_bindings.get_key_for_action(2))
  {
    return { create_press_move_action() };
  }
  else if (key_pressed == m_key_bindings.get_key_for_action(3))
  {
    return { create_press_attack_action() };
  }
  else if (key_pressed == m_key_bindings.get_key_for_action(4))
  {
    // Nothing yet
  }
  return {};
}

std::vector<control_action> controller::process_mouse_pressed(
  const sf::Event& event,
  const game& g
) const
{
  assert(event.type == sf::Event::MouseButtonPressed);
  const auto mouse_screen_pos{
    screen_coordinat(event.mouseButton.x, event.mouseButton.y)
  };
  if (event.mouseButton.button == sf::Mouse::Left)
  {
    return {
      create_press_lmb_action(
        convert_to_game_coordinat(
          mouse_screen_pos,
          g.get_layout()
        )
      )
    };
  }
  assert(event.mouseButton.button == sf::Mouse::Right);
  return {
    create_press_rmb_action(
      convert_to_game_coordinat(
        mouse_screen_pos,
        g.get_layout()
      )
    )
  };
}

std::vector<control_action> controller::process_mouse_moved(
  const sf::Event& event,
  const game& g
) const
{
  assert(event.type == sf::Event::MouseMoved);
  const auto mouse_screen_pos{
    screen_coordinat(event.mouseMove.x, event.mouseMove.y)
  };
  const auto mouse_game_pos{
    convert_to_game_coordinat(
      mouse_screen_pos,
      g.get_layout()
    )
  };
  return { create_mouse_move_action(mouse_game_pos) };
}

void test_controller()
{
#ifndef NDEBUG
  // controller::get_player
  {
    const controller c(controller_type::mouse, side::lhs);
    assert(c.get_player() == side::lhs);
    const controller d(controller_type::keyboard, side::rhs);
    assert(d.get_player() == side::rhs);
  }
  // controller::get_type
  {
    const controller c(controller_type::mouse, side::lhs);
    assert(c.get_type() == controller_type::mouse);
    const controller d(controller_type::keyboard, side::lhs);
    assert(d.get_type() == controller_type::keyboard);
  }
  // press up does nothing with a mouse
  {
    const game g;
    const controller c(controller_type::mouse, side::lhs);
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_up()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(actions.empty());
  }
  // press up works with a keyboard
  {
    const game g;
    const controller c(controller_type::keyboard, side::lhs);
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_up()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press right works with a keyboard
  {
    const game g;
    const controller c(controller_type::keyboard, side::lhs);
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_right()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press down works with a keyboard
  {
    const game g;
    const controller c(controller_type::keyboard, side::lhs);
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_down()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press left works with a keyboard
  {
    const game g;
    const controller c(controller_type::keyboard, side::lhs);
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_left()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press action 1 works with a keyboard
  {
    const game g;
    const controller c(controller_type::keyboard, side::lhs);
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(1)
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press action 2 works with a keyboard
  {
    const game g;
    const controller c(controller_type::keyboard, side::lhs);
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(2)
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press action 3 works with a keyboard
  {
    const game g;
    const controller c(controller_type::keyboard, side::lhs);
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(3)
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press action 4 does nothing yet
  {
    const game g;
    const controller c(controller_type::keyboard, side::lhs);
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(4)
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(actions.empty());
  }

  {
    const game g;
    const controller c(controller_type::mouse, side::lhs);
    sf::Event e;
    e.type = sf::Event::KeyPressed;
    assert(c.get_type() == controller_type::mouse);
    assert(c.process_input(e, g).empty());
  }
#endif
}
