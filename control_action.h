#ifndef CONTROL_ACTION_H
#define CONTROL_ACTION_H

#include "ccfwd.h"

#include "control_action_type.h"
//#include "game_coordinat.h"
#include "square.h"

/// An action
class control_action
{
public:
  control_action(const control_action_type type, const square& coordinat);
  auto& get_coordinat() const noexcept { return m_coordinat; }
  auto get_type() const noexcept { return m_type; }

private:
  square m_coordinat;
  control_action_type m_type;
};

control_action create_mouse_move_action(const square& coordinat);
control_action create_press_attack_action();
control_action create_press_down_action();
control_action create_press_left_action();
control_action create_press_lmb_action(const square& coordinat);
control_action create_press_move_action();
control_action create_press_right_action();
control_action create_press_rmb_action(const square& coordinat);
control_action create_press_select_action();
control_action create_press_up_action();

/// Test the 'control_action' class and its free functions
void test_control_actions();

#endif // CONTROL_ACTION_H

