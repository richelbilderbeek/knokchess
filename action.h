#ifndef ACTION_H
#define ACTION_H

#include "action_type.h"
#include "game_coordinat.h"

/// An action
class action
{
public:
  action(const action_type type, const game_coordinat& coordinat);
  auto& get_coordinat() const noexcept { return m_coordinat; }
  auto get_type() const noexcept { return m_type; }

private:
  game_coordinat m_coordinat;
  action_type m_type;
};

action create_mouse_move_action(const game_coordinat& coordinat);
action create_press_attack_action();
action create_press_down_action();
action create_press_left_action();
action create_press_lmb_action(const game_coordinat& coordinat);
action create_press_move_action();
action create_press_right_action();
action create_press_rmb_action(const game_coordinat& coordinat);
action create_press_select_action();
action create_press_up_action();


#endif // ACTION_H

