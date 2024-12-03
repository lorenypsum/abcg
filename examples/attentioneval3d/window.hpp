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
#include "distraction_objects.hpp"
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
  // Funções de ciclo de vida do jogo
  void initializeGameObjects();
  void checkGameStatus();
  void updateTimeDisplay();
  void resetGame();
  void startGame();

private:
  // Programa de shader
  GLuint m_program{};

  // Objects
  Model m_model;
  DistractionObjects m_distractions;
  GameData m_gameData;

  // Shaders
  std::vector<char const *> m_shaderNames{
      "cubereflect", "cuberefract", "normalmapping", "texture", "blinnphong",
      "phong",       "gouraud",     "normal",        "depth"};
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

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
  glm::ivec2 m_viewportSize;
  float m_FOV{30.0f};
  ImFont *m_font{};
};

#endif