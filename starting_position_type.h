#ifndef STARTING_POSITION_TYPE_H
#define STARTING_POSITION_TYPE_H

#include <string>

/// The starting position type
enum class starting_position_type
{
  standard,
  kings_only,
  before_scholars_mate,
  bishop_and_knight_end_game
};

constexpr starting_position_type get_default_starting_position() { return starting_position_type::standard; }

/// Get the next starting position,
/// i.e. when the user presses right
starting_position_type get_next(const starting_position_type starting_position) noexcept;

/// Test this class and its free functions
void test_starting_position_type();

std::string to_str(const starting_position_type t) noexcept;

#endif // STARTING_POSITION_TYPE_H
