#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "control_action.h"
#include "controller_type.h"

#include <SFML/Window/Event.hpp>

/// A controller is a class that takes input from
/// a mouse/keyboard/AI and converts these into
/// the right 'control_action'
class controller
{
public:
  controller(const controller_type type);

  const auto& get_type() const noexcept { return m_type; }

  /// Process the input for this controller.
  /// Returns an an empty vector if this controller is uneffected by the event
  std::vector<control_action> process_input(
   const sf::Event& event,
   const game& g
 ) const;

private:

  controller_type m_type;

  /// Process a key press from a keyboard
  std::vector<control_action> process_key_press(
    const sf::Event& event
  ) const;

  /// Process the input from a mouse button press
  std::vector<control_action> process_mouse_pressed(
    const sf::Event& event,
    const game& g
  ) const;

  /// Process the input from a mouse movement
  std::vector<control_action> process_mouse_moved(
    const sf::Event& event,
    const game& g
  ) const;
};

/// Test this class and its free functions
void test_controller();

#endif // CONTROLLER_H
