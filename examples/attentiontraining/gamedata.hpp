#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class GameState { Playing, GameOver, Win, Start }; // Adicionando o estado de início

struct GameData {
  GameState m_state{GameState::Start}; // Estado inicial do jogo
};

#endif


