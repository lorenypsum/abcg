#ifndef WINDOW_HPP_
#define WINDOW_HPP_

// Bibliotecas
#include "abcgOpenGL.hpp"
#include <chrono>
#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <random>
#include <vector>

// Classes
#include "game_objects.hpp"
#include "gamedata.hpp"
#include "model.hpp"

// Classe que representa a janela do jogo
class Window : public abcg::OpenGLWindow {
protected:
  // Funções de window
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onEvent(const SDL_Event &event) override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

public:
  // Funções de ciclo da aplicação 3d (jogo)
  void initializeGameObjects();
  void checkGameStatus();
  void updateTimeDisplay();
  void resetGame();
  void startGame();
  void detectCollisions();


private:
  // // Programa de shader
  // GLuint m_program{};
  // GLuint m_program_obj{};

  // Variáveis de viewport
  glm::ivec2 m_viewportSize;

  // Classes Instanciadas
  GameEntities m_objects;
  GameData m_gameData;
  Model m_ground;

  // Variáveis do jogo
  int m_score{0};         // Pontuação do jogador
  int m_lastScore{0};     // Pontuação anterior
  int m_newScore{0};      // Nova pontuação
  int m_gametime{30};     // Tempo de jogo
  int m_restartDelay{40}; // Tempo de espera antes de reiniciar o jogo
  bool m_gameOver{false}; // Flag que indica se o jogo acabou

  // Variáveis
  std::default_random_engine m_randomEngine;
  std::chrono::steady_clock::time_point m_lastReload;
  float m_FOV{30.0f};
  ImFont *m_font{};
};

#endif