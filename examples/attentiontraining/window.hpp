#ifndef WINDOW_HPP_
#define WINDOW_HPP_
#include "distraction_objects.hpp"
#include "target_objects.hpp"

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"
#include <chrono>
#include <random>
#include <vector>

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onEvent(SDL_Event const &event) override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

public:
   // Funções de inicialização e controle
  void initializeGameObjects();
  void checkGameStatus();
  void updateLivesDisplay();
  void resetGame();

private:
  // Variáveis de textura e programa
  GLuint m_program{};
  GLuint m_targetTexture{};
  GLuint m_distractionTexture{};
  GLuint m_objectsProgram; // Programa de shader para renderizar objetos
  GameData m_gameData;     // Dados do jogo (estado, vidas, etc.)           //
                           // Objeto cena que contém os elementos do jogo

  int m_score{0};         // Pontuação do jogador
  int m_lives{3};         // Vidas do jogador
  bool m_gameOver{false}; // Flag que indica se o jogo acabou

  // Objetos de distração e alvo
  DistractionObjects m_distractionObjects;
  TargetObjects m_targetObject;

  std::default_random_engine m_randomEngine; // Gerador de números aleatórios
  std::chrono::steady_clock::time_point
      m_lastReload;             // Marca do último recarregamento de objetos
  glm::ivec2 m_viewportSize;    // Tamanho da janelaC
};

#endif
