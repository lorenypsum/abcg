#ifndef WINDOW_HPP_
#define WINDOW_HPP_
#include "background.hpp"
#include "distraction_objects.hpp"
#include "target_objects.hpp"

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"
#include <chrono>
#include <random>
#include <vector>

class Window : public abcg::OpenGLWindow {
protected:
  // Funções de ciclo de vida do jogo
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onEvent(SDL_Event const &event) override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

public:
  // Funções de inicialização e atualização do jogo
  void initializeGameObjects();
  void checkGameStatus();
  void updateTimeDisplay();
  void resetGame();
  void startGame();

private:
  // Programa de shader
  GLuint m_program{};
  GLuint m_targetTexture{};
  GLuint m_distractionTexture{};
  GLuint m_objectsProgram;
  GameData m_gameData;

  int m_score{0};         // Pontuação do jogador
  int m_lastScore{0};     // Pontuação anterior
  int m_newScore{0};      // Nova pontuação
  int m_gametime{30};     // Tempo de jogo
  int m_restartDelay{40}; // Tempo de espera antes de reiniciar o jogo
  bool m_gameOver{false}; // Flag que indica se o jogo acabou

  // Objetos de distração e alvo
  DistractionObjects m_distractionObjects;
  TargetObjects m_targetObject;

  // Instância de Background
  Background m_background;
  ImFont *m_font{};
  std::default_random_engine m_randomEngine;
  std::chrono::steady_clock::time_point m_lastReload;
  glm::ivec2 m_viewportSize;
};

#endif
