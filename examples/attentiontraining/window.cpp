#include "window.hpp"
#include "gamedata.hpp"
#include <chrono>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <imgui.h>
#include <random>

// Inicializa o jogo e configura o programa de shaders
void Window::onCreate() {
  // Carrega o shader program para renderização dos objetos
  auto const assetsPath{abcg::Application::getAssetsPath()};
  // Cria o programa de shader
  m_program = abcg::createOpenGLProgram({
      {.source = assetsPath + "/shaders/texture.vert",
       .stage = abcg::ShaderStage::Vertex},
      {.source = assetsPath + "/shaders/texture.frag",
       .stage = abcg::ShaderStage::Fragment},
  });

  // Inicializa o cenário
  m_background.create();

  // Carrega a fonte
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  // Inicializa texturas
  m_targetTexture =
      m_targetObject.loadTexture(assetsPath + "/textures/whitepigeon.png");
  m_distractionTexture = m_distractionObjects.loadTexture(
      assetsPath + "/textures/brownpigeon.png");

  // Configura estado inicial
  startGame();
  m_randomEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
}

// Desenha os objetos de distração e o alvo
void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  // Desenha o cenário
  m_background.paint();

  // Renderiza objetos
  m_targetObject.paint();
  m_distractionObjects.paint();
}

// Renderiza a interface do usuário, exibindo score e vidas
void Window::onPaintUI() {
  // Renderiza a interface do usuário
  abcg::OpenGLWindow::onPaintUI();
  {
    // Configura a janela de interface do usuário
    auto const size{ImVec2(800, 600)};
    auto const position{ImVec2(170.0f, 1.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);
    // Exibe a pontuação e o tempo restante
    if (m_gameData.m_state == GameState::Playing) {
      ImGui::Text("Score: %d Time: %d", m_score, m_gametime);
    } else if (m_gameData.m_state == GameState::GameOver) {
      ImGui::Text("Your Score: %d", m_newScore);
    } else if (m_gameData.m_state == GameState::Win) {
      ImGui::Text("Congratulations! New Record: %d", m_newScore);
    } else if (m_gameData.m_state == GameState::Start) {
      ImGui::Text("Pegue o Pombo Branco!");
    } else {
      ImGui::Text("...");
    }
    // Finaliza a janela de interface do usuário
    ImGui::PopFont();
    ImGui::End();
  }
}

// Trata eventos de clique do mouse e toques no touchpad para interagir com os
// objetos
void Window::onEvent(SDL_Event const &event) {
  glm::vec2 clickPos;

  // Verifica se o evento é um clique do mouse
  if (event.type == SDL_MOUSEBUTTONDOWN &&
      event.button.button == SDL_BUTTON_LEFT) {
    clickPos = {
        event.button.x / static_cast<float>(m_viewportSize.x) * 2.0f - 1.0f,
        1.0f - event.button.y / static_cast<float>(m_viewportSize.y) * 2.0f};

  } else if (event.type == SDL_FINGERDOWN) {
    // Converte as coordenadas do toque em um valor normalizado
    clickPos = {event.tfinger.x * 2.0f -
                    1.0f, // SDL_FINGERDOWN usa valores normalizados de 0 a 1
                1.0f - event.tfinger.y * 2.0f};
  } else {
    // Se não for um evento de interesse, retorna
    return;
  }

  // Verifica se o clique/toque foi em um objeto de distração ou no alvo
  if (m_targetObject.checkClickOnTarget(clickPos)) {
    m_score += 1; // Ganha um ponto ao clicar no alvo correto
  } else if (m_distractionObjects.checkClickOnDistraction(clickPos)) {
    m_score -= 1;        // Perde um ponto ao clicar em um objeto de distração
    updateTimeDisplay(); // Atualiza a exibição de tempo
  }
}

// Atualiza o jogo a cada frame, verifica e atualiza o status do jogo
void Window::onUpdate() {
  // Calcula o tempo decorrido desde o último recarregamento
  auto currentTime = std::chrono::steady_clock::now();
  float elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(
                          currentTime - m_lastReload)
                          .count();

  // Recarrega objetos a cada 3 segundos
  if (elapsedTime >= 1.0f && m_gameData.m_state == GameState::Playing) {
    --m_gametime;                             // Decrementa o tempo restante
    m_distractionObjects.update(elapsedTime); // Atualiza objetos de distração
    m_lastReload = currentTime;               // Atualiza o tempo de recarga
    checkGameStatus();                        // Verifica o status do jogo
    updateTimeDisplay();                      // Atualiza a exibição de tempo
  }
  checkGameStatus(); // Verifica o status do jogo
}

// Ajusta o tamanho do viewport ao redimensionar a janela
void Window::onResize(glm::ivec2 const &size) { m_viewportSize = size; }

// Destroi recursos ao finalizar o jogo
void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  m_targetObject.destroy();
  m_distractionObjects.destroy();
  m_background.destroy();
}

// Inicializa os objetos do jogo em posições aleatórias
void Window::initializeGameObjects() {
  // Recarrega a posição do alvo e dos objetos de distração
  m_targetObject.create(m_program, 1);
  m_distractionObjects.create(m_program, 6);
}

// Verifica o status do jogo e atualiza o estado conforme necessário
void Window::checkGameStatus() {
  // Estado inicial, somente se o tempo está no valor inicial e pontuação for 0
  if (m_gametime == 30 && m_score == 0 &&
      m_gameData.m_state != GameState::Playing) {
    m_gameOver = false;
    m_gameData.m_state = GameState::Start;
  }
  // Verifica se o tempo terminou e define o estado de Game Over ou Vitória
  else if (m_gametime <= 0) {
    if (m_score > m_lastScore) {
      m_lastScore = m_score;
      m_newScore = m_score;
      m_gameData.m_state = GameState::Win; // Estado de vitória
      m_gameOver = true;
    } else {
      m_newScore = m_score;
      m_gameData.m_state = GameState::GameOver; // Estado de game over
      m_gameOver = true;
    }
  }
  // Define o estado como "Playing" se o jogo ainda está em andamento
  else if (m_gametime > 0 && m_gameData.m_state != GameState::Win &&
           m_gameData.m_state != GameState::GameOver && m_score > 0) {
    m_gameOver = false;
    m_gameData.m_state = GameState::Playing;
  }
}

// Atualiza a exibição de tempo na interface do usuário
void Window::updateTimeDisplay() {
  if (m_gametime <= 0) {
    m_newScore = m_score;
    m_gameOver = true;
    m_gameData.m_state = GameState::GameOver;
  }
}

// Reinicia o jogo para o estado inicial
void Window::resetGame() {
  if (m_restartDelay < 0) {
    m_score = 0;
    m_lastScore = 0;
    m_newScore = 0;
    m_gametime = 30;
    m_gameOver = false;
    m_restartDelay = 40;
    m_gameData.m_state =
        GameState::Start; // Começa do estado Start
                          // Recria os objetos e reinicia o tempo de recarga
    m_lastReload = std::chrono::steady_clock::now();
    initializeGameObjects();
  }
}

// Reinicia o jogo para o estado inicial
void Window::startGame() {
  m_score = 0;
  m_gametime = 30;
  m_gameOver = false;
  m_restartDelay = 40;
  m_gameData.m_state = GameState::Start; // Começa do estado Start

  // Recria os objetos e reinicia o tempo de recarga
  m_lastReload = std::chrono::steady_clock::now();
  initializeGameObjects();
}