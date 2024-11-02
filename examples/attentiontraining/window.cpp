#include "window.hpp"
#include "gamedata.hpp"
#include <chrono>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <imgui.h>
#include <random>

// Inicializa o jogo e configura o programa de shaders
void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  m_program = abcg::createOpenGLProgram({
      {.source = assetsPath + "/shaders/texture.vert",
       .stage = abcg::ShaderStage::Vertex},
      {.source = assetsPath + "/shaders/texture.frag",
       .stage = abcg::ShaderStage::Fragment},
  });

  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  m_targetTexture =
      m_targetObject.loadTexture(assetsPath + "/textures/banana.png");
  m_distractionTexture = m_distractionObjects.loadTexture(
      assetsPath + "/textures/basket_yellow.png");

  resetGame();
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  m_targetObject.paint();
  m_distractionObjects.paint();
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  auto const size{ImVec2(800, 600)};
  auto const position{ImVec2(200.0f, 1.0f)};
  ImGui::SetNextWindowPos(position);
  ImGui::SetNextWindowSize(size);
  ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground |
                               ImGuiWindowFlags_NoTitleBar |
                               ImGuiWindowFlags_NoInputs};
  ImGui::Begin(" ", nullptr, flags);
  ImGui::PushFont(m_font);

  switch (m_gameData.m_state) {
  case GameState::Playing:
    ImGui::Text("Score: %d Lives: %d", m_score, m_lives);
    break;
  case GameState::GameOver:
    ImGui::Text("Game Over! Press to Restart");
    break;
  case GameState::Win:
    ImGui::Text("*You Win!* Press to Restart");
    break;
  case GameState::Start:
    ImGui::Text("Pegue Bananas!");
    break;
  default:
    ImGui::Text("...");
    break;
  }

  ImGui::PopFont();
  ImGui::End();

  // Reinicia o jogo se o jogador clicar para reiniciar
  if ((m_gameData.m_state == GameState::GameOver ||
       m_gameData.m_state == GameState::Win) &&
      ImGui::IsMouseClicked(0)) {
    resetGame();
  }
}

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_MOUSEBUTTONDOWN &&
      event.button.button == SDL_BUTTON_LEFT) {
    glm::vec2 clickPos{
        event.button.x / static_cast<float>(m_viewportSize.x) * 2.0f - 1.0f,
        1.0f - event.button.y / static_cast<float>(m_viewportSize.y) * 2.0f};

    if (m_targetObject.checkClickOnTarget(clickPos)) {
      m_score += 1;
    } else if (m_distractionObjects.checkClickOnDistraction(clickPos)) {
      --m_lives;
      updateLivesDisplay();
    }
  }
}

void Window::onUpdate() {
  auto currentTime = std::chrono::steady_clock::now();
  float elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(
                          currentTime - m_lastReload)
                          .count();

  if (elapsedTime >= 2.0f) {
    m_distractionObjects.update(elapsedTime);
    m_lastReload = currentTime;
  }
  checkGameStatus();
}

void Window::onResize(glm::ivec2 const &size) { m_viewportSize = size; }

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  m_targetObject.destroy();
  m_distractionObjects.destroy();
}

void Window::initializeGameObjects() {
  m_targetObject.create(m_program, 1);
  m_distractionObjects.create(m_program, 3);
}

void Window::checkGameStatus() {
  if (m_lives <= 0) {
    m_gameData.m_state = GameState::GameOver;
  } else if (m_score >= 10) {
    m_gameData.m_state = GameState::Win;
  } else if (m_score == 0 && m_lives == 10) {
    m_gameData.m_state = GameState::Start;
  } else {
    m_gameData.m_state = GameState::Playing;
  }
}

void Window::updateLivesDisplay() {
  if (m_lives <= 0) {
    m_gameData.m_state = GameState::GameOver;
  }
}

void Window::resetGame() {
  m_score = 0;
  m_lives = 10;
  m_gameData.m_state = GameState::Start;

  m_targetObject.destroy();
  m_distractionObjects.destroy();

  m_lastReload = std::chrono::steady_clock::now();
  initializeGameObjects();
}
