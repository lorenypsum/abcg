#include "window.hpp"

// Converte um valor hexadecimal em um vetor de 3 componentes
glm::vec3 hexToVec3(uint32_t hexValue) {
  float red = ((hexValue >> 16) & 0xFF) / 255.0f;
  float green = ((hexValue >> 8) & 0xFF) / 255.0f;
  float blue = (hexValue & 0xFF) / 255.0f;

  return glm::vec3(red, green, blue);
}

// Cores
glm::vec3 yinmnBlue = hexToVec3(0x465775);
glm::vec3 skyblue = hexToVec3(0xB4DEF5);

// Inicializa a janela
void Window::onCreate() {

  abcg::glClearColor(skyblue.r, skyblue.g, skyblue.b, 1.0f); // Cor de fundo

  auto const assetsPath{
      abcg::Application::getAssetsPath()}; // Caminho dos assets

  // Carrega a fonte
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 25.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Não é possível carregar o arquivo de fonte");
  }

  // Configura estado inicial
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  // Inicializa os objetos do jogo
  m_objects.create();

  // Inicializa o Jogo
  startGame();
}

// Atualiza a janela
void Window::onUpdate() {

  // Calcula o tempo decorrido desde a última atualização
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Atualiza objetos de cena
  m_objects.update(deltaTime);

  // Calcula o tempo decorrido desde o último recarregamento
  auto currentTime = std::chrono::steady_clock::now();
  float elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(
                          currentTime - m_lastReload)
                          .count();

  // Recarrega objetos a cada 3 segundos
  if (elapsedTime >= 1.0f && m_gameData.m_state == GameState::Playing) {
    --m_gametime;               // Decrementa o tempo restante
    m_lastReload = currentTime; // Atualiza o tempo de recarga
    checkGameStatus();          // Verifica o status do jogo
    // Atualiza a lógica de colisões
    detectCollisions();
    updateTimeDisplay(); // Atualiza a exibição de tempo
  }
  // Atualiza a lógica de colisões
  detectCollisions();
  checkGameStatus(); // Verifica o status do jogo
}

// Função para detectar colisões
void Window::detectCollisions() {
    // A rede possui uma posição e um tamanho (vamos assumir um raio ou largura e altura)
    glm::vec3 netPosition = m_objects.m_net.m_position; 
    float netSize = 1.0f; // Tamanho da rede, pode ser ajustado

    // Detecta colisões com targets
    for (auto &target : m_objects.m_targetObjects) {
        glm::vec3 targetPosition = target.m_position;
        float targetSize = target.m_size; // Tamanho do target, pode ser ajustado

        // Distância entre a rede e o target
        float distance = glm::distance(netPosition, targetPosition);

        // Se a distância for menor que a soma dos raios, houve uma colisão
        if (distance < (netSize + targetSize)) {
            // A colisão com o target aumenta a pontuação
            m_score += 1;
            // Reseta ou remove o target após a colisão
            target.m_position = glm::vec3(-1000.0f, -1000.0f, 0.0f); // Exemplo de remoção do target
        }
    }

    // Detecta colisões com distractions
    for (auto &distraction : m_objects.m_distractionObjects) {
        glm::vec3 distractionPosition = distraction.m_position;
        float distractionSize = distraction.m_size; // Tamanho do distraction, pode ser ajustado

        // Distância entre a rede e o distraction
        float distance = glm::distance(netPosition, distractionPosition);

        // Se a distância for menor que a soma dos raios, houve uma colisão
        if (distance < (netSize + distractionSize)) {
            // A colisão com o distraction diminui a pontuação
            m_score -= 1;
            // Reseta ou remove o distraction após a colisão
            distraction.m_position = glm::vec3(-1000.0f, -1000.0f, 0.0f); // Exemplo de remoção do distraction
        }
    }
}

// Renderiza a janela
// TODO: Renderizar o chão/solo
void Window::onPaint() { m_objects.paint(); }

// Renderiza a interface do usuário
void Window::onPaintUI() {

  abcg::OpenGLWindow::onPaintUI();
  {
    // Configura a janela de interface do usuário
    auto const size{ImVec2(600, 67)};              // Tamanho da janela
    auto const position{ImVec2(size.x / 2, 5.0f)}; // Posição da janela
    ImGui::SetNextWindowPos(position); // Configura a posição da janela
    ImGui::SetNextWindowSize(size);    // Configura o tamanho da janela
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);
    {
      ImGui::StyleColorsDark();        // Estilo escuro
      ImGui::SetWindowFontScale(1.0f); // Escala da fonte
      ImGui::PushStyleColor(ImGuiCol_Text,
                            ImVec4(1.0, 1.0, 1.0, 1.0f)); // Cor do texto
      ImGui::PushStyleColor(
          ImGuiCol_WindowBg,
          ImVec4(yinmnBlue.r, yinmnBlue.g, yinmnBlue.b, 1.0f)); // Cor de fundo
      ImGui::Indent((size.x / 4)); // Identação do texto
      ImGui::PushFont(m_font);     // Inclui Fonte

      // Exibe a pontuação e o tempo restante
      if (m_gameData.m_state == GameState::Playing) {
        ImGui::Text("Pontuação: %d Tempo: %d", m_score,
                    m_gametime); // Texto de pontuação e tempo
      } else if (m_gameData.m_state == GameState::GameOver) {
        ImGui::Text("Pontuação final: %d",
                    m_newScore); // Texto de pontuação final
      } else if (m_gameData.m_state == GameState::Win) {
        ImGui::Text("Parabéns! Novo Recorde: %d",
                    m_newScore); // Texto de novo recorde
      } else if (m_gameData.m_state == GameState::Start) {
        ImGui::Text("Pegue os pássaros brancos!"); // Texto de instrução
        ImGui::Text("Pontuação: %d Tempo: %d", m_score,
                    m_gametime); // Texto de pontuação e tempo
      } else {
        ImGui::Text("..."); // Texto de DEFAULT
      }

      // Finaliza a janela de interface do usuário
      ImGui::PopFont();
      ImGui::End();
    }
  }

  m_objects.paintUI(); // Renderiza a interface dos objetos
}

// Atualiza o tamanho da janela
void Window::onResize(glm::ivec2 const &size) {
  m_objects.m_viewportSize = size;
}

// Eventos de clique do mouse
// TODO: Incluir toque na tela
void Window::onEvent(SDL_Event const &event) {
  // if (event.type == SDL_MOUSEBUTTONDOWN &&
  //     event.button.button == SDL_BUTTON_LEFT) {
  //   if (!m_gameOver) {
  //     // Converte a posição do clique para coordenadas normalizadas
  //     auto const clickPos = glm::vec3{
  //         (event.button.x / static_cast<float>(m_viewportSize.x)) * 2.0f - 1.0f,
  //         -((event.button.y / static_cast<float>(m_viewportSize.y)) * 2.0f -
  //           1.0f),
  //         0.0f};

  //     // Verifica cliques em targets ou distractions
  //     if (m_objects.checkCollisionWithNet(m_objects.m_net.m_position, 2.0f,
  //                                         m_objects.m_targetObjects, 2.0f)) {
  //       m_score += 1; // Pontuação aumenta para cliques corretos
  //     } else if (m_objects.checkCollisionWithNet(
  //                    m_objects.m_net.m_position, 2.0f,
  //                    m_objects.m_targetObjects, 2.0f)) {
  //       m_score -= 1; // Pontuação diminui para cliques incorretos
  //     }
  //   }
  // }

  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_UP: // Mover para cima
      m_objects.m_net.m_position.y += 0.1f;
      break;
    case SDLK_DOWN: // Mover para baixo
      m_objects.m_net.m_position.y -= 0.1f;
      break;
    case SDLK_LEFT: // Mover para a esquerda
      m_objects.m_net.m_position.x -= 0.1f;
      break;
    case SDLK_RIGHT: // Mover para a direita
      m_objects.m_net.m_position.x += 0.1f;
      break;
    }
  }

  // Limites para evitar que o astronauta saia da tela
  m_objects.m_net.m_position.x =
      std::clamp(m_objects.m_net.m_position.x, -0.5f, 0.5f);
  m_objects.m_net.m_position.y =
      std::clamp(m_objects.m_net.m_position.y, -0.5f, 0.5f);
}

// Inicializa os objetos do jogo em posições aleatórias
void Window::initializeGameObjects() {
  m_objects.create(); // Recarrega a posição do alvo e dos objetos de distração
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
    m_gameData.m_state = GameState::Playing; // Estado de jogo em andamento
  }
}

// Atualiza a exibição de tempo na interface do usuário
void Window::updateTimeDisplay() {
  if (m_gametime <= 0) {
    m_newScore = m_score;
    m_gameOver = true;
    m_gameData.m_state = GameState::GameOver; // Estado de game over
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
    m_gameData.m_state = GameState::Start; // Começa do estado Start
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

// Destroi a janela
void Window::onDestroy() { m_objects.destroy(); }