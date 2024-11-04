#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

// Estados do jogo
enum class GameState { Playing, GameOver, Win, Start };

// Estado inicial do jogo
struct GameData {
  GameState m_state{GameState::Start};
};

#endif
