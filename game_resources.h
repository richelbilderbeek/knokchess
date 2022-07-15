#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#ifndef LOGIC_ONLY

#include "chess_color.h"
#include "piece_type.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class game_resources
{
public:
  game_resources();

  /// Get a picture of a black/dark square
  sf::Texture& get_black_square() noexcept { return m_black_square; }

  /// Get texture of a piece
  sf::Texture& get_piece(
    const chess_color color,
    const piece_type type
  );

  /// Get texture of a portrait of a piece
  sf::Texture& get_piece_portrait(
    const chess_color color,
    const piece_type type
  );

  /// Get a picture of a white/light square
  sf::Texture& get_white_square() noexcept { return m_white_square; }

  /// Get a font
  sf::Font& get_font() noexcept {return m_font; }

  sf::Music& get_ninja_gods() noexcept { return m_ninja_gods; }

  /// Get texture of a piece
  void play_sound(
    const chess_color color,
    const piece_type type
  );

private:
  sf::Texture m_black_bishop;
  sf::Texture m_black_bishop_portrait;
  sf::Texture m_black_king;
  sf::Texture m_black_king_portrait;
  sf::Texture m_black_knight;
  sf::Texture m_black_knight_portrait;
  sf::Texture m_black_pawn;
  sf::Texture m_black_pawn_portrait;
  sf::Texture m_black_queen;
  sf::Texture m_black_queen_portrait;
  sf::Texture m_black_rook;
  sf::Texture m_black_rook_portrait;
  sf::Texture m_black_square;
  sf::Texture m_white_bishop;
  sf::Texture m_white_bishop_portrait;
  sf::Texture m_white_king;
  sf::Texture m_white_king_portrait;
  sf::Texture m_white_knight;
  sf::Texture m_white_knight_portrait;
  sf::Texture m_white_queen;
  sf::Texture m_white_queen_portrait;
  sf::Texture m_white_pawn;
  sf::Texture m_white_pawn_portrait;
  sf::Texture m_white_rook;
  sf::Texture m_white_rook_portrait;
  sf::Texture m_white_square;
  sf::Font m_font;
  sf::Music m_ninja_gods;

  sf::Sound m_faring_into_battle;
  sf::Sound m_its_time_to_rock;
  sf::Sound m_jumping_into_battle;
  sf::Sound m_lets_rule;
  sf::Sound m_moving_forward;
  sf::Sound m_to_rule_is_to_act;

  sf::SoundBuffer m_faring_into_battle_buffer;
  sf::SoundBuffer m_its_time_to_rock_buffer;
  sf::SoundBuffer m_jumping_into_battle_buffer;
  sf::SoundBuffer m_lets_rule_buffer;
  sf::SoundBuffer m_moving_forward_buffer;
  sf::SoundBuffer m_to_rule_is_to_act_buffer;
};

/// Test the game resources
void test_game_resources();

#endif // LOGIC_ONLY

#endif // GAME_RESOURCES_H
