#include "camera.hpp"

#include "abcg.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Camera::computeViewMatrix() {
  // Atualiza a direção da câmera com base na rotação do playera
  glm::vec3 direction =
      glm::vec3(glm::sin(m_playerAngle), 0.0f, glm::cos(m_playerAngle));

  // A posição da câmera é ajustada para estar sempre atrás do jogador
  m_at = m_eye + direction;

  // Cria a matriz de visualização usando a posição da câmera (m_eye) e o ponto
  // para onde ela está olhando (m_at)
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projMatrix =
      glm::perspective(glm::radians(45.0f), size.x / size.y, 0.1f, 100.0f);
}

void Camera::resizeViewport(glm::ivec2 const &size) {
  // m_viewportSize = size;
  m_viewportSize = size;
  computeProjectionMatrix(glm::vec2{size.x, size.y});
}

void Camera::mousePress(glm::ivec2 const &position) {
  m_mouseTracking = true;
  m_lastMousePosition = glm::vec3(position.x, position.y, 0.0f);
}

void Camera::mouseMove(glm::ivec2 const &position) {
  if (m_mouseTracking) {
    glm::vec2 mouseDelta =
        glm::vec2(position.x, position.y) -
        glm::vec2(m_lastMousePosition.x, m_lastMousePosition.y);
    m_lastMousePosition = glm::vec3(position.x, position.y, 0.0f);

    float sensitivity = 0.05f;
    m_playerAngle += mouseDelta.x * sensitivity;   // Ajuste do yaw
    m_verticalAngle -= mouseDelta.y * sensitivity; // Ajuste do pitch
    if (m_verticalAngle > glm::radians(89.0f))
      m_verticalAngle = glm::radians(89.0f);
    if (m_verticalAngle < glm::radians(-89.0f))
      m_verticalAngle = glm::radians(-89.0f);

    // Atualizar a posição da câmera com os novos ângulos
    m_eye.x = m_playerDistance * cos(m_verticalAngle) * sin(m_playerAngle);
    m_eye.y = m_playerDistance * sin(m_verticalAngle);
    m_eye.z = m_playerDistance * cos(m_verticalAngle) * cos(m_playerAngle);
    m_at = {0.0f, 0.0f, 0.0f};
    computeViewMatrix();
  }
}

void Camera::mouseRelease() { m_mouseTracking = false; }

void Camera::update(glm::vec3 position, SDL_Event &event, float deltaTime) {
  // Atualiza a posição da câmera com base na posição do jogador
  m_eye = position + glm::vec3(0.0f, 1.5f, -5.0f);

  // Processa os eventos de teclado para movimentação e rotação
  processKeyboard(event, deltaTime);

  // Atualiza a matriz de visualização da câmera
  computeViewMatrix();
}

void Camera::processKeyboard(SDL_Event &event, float deltaTime) {
  const float velocity = 5.0f * deltaTime; // Velocidade de movimentação

  // Processa os eventos de teclado
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.scancode) {
      // Movimento do jogador
      case SDL_SCANCODE_W: // Move para frente
        m_eye += glm::normalize(m_at - m_eye) * velocity;
        break;
      case SDL_SCANCODE_S: // Move para trás
        m_eye -= glm::normalize(m_at - m_eye) * velocity;
        break;
      case SDL_SCANCODE_A: // Move para a esquerda
        m_eye -= glm::normalize(glm::cross(m_at - m_eye, m_up)) * velocity;
        break;
      case SDL_SCANCODE_D: // Move para a direita
        m_eye += glm::normalize(glm::cross(m_at - m_eye, m_up)) * velocity;
        break;

      // Rotação da câmera
      case SDL_SCANCODE_LEFT: // Rotação para a esquerda
        m_playerAngle += 0.05f;
        break;
      case SDL_SCANCODE_RIGHT: // Rotação para a direita
        m_playerAngle -= 0.05f;
        break;

      default:
        break;
      }
      break;
    default:
      break;
    }
  }
}

void Camera::zoom(float scroll) {
  m_playerDistance -= scroll * 0.1f;
  if (m_playerDistance < 2.0f)
    m_playerDistance = 2.0f; // Limitar distância mínima
  if (m_playerDistance > 20.0f)
    m_playerDistance = 20.0f; // Limitar distância máxima
}

void Camera::rotateAroundPlayer(float angleVariation) {
  m_playerAngle += angleVariation; // Girar ao redor do playera
  m_eye.x = m_playerDistance * cos(glm::radians(m_playerAngle));
  m_eye.z = m_playerDistance * sin(glm::radians(m_playerAngle));
  computeViewMatrix();
}

glm::vec3 Camera::project(glm::vec2 const &mousePosition) const {
  glm::vec4 viewport =
      glm::vec4(0.0f, 0.0f, m_viewportSize.x, m_viewportSize.y);
  glm::mat4 modelViewMatrix = getViewMatrix();
  glm::mat4 projectionMatrix = getProjMatrix();

  glm::vec3 result =
      glm::unProject(glm::vec3(mousePosition, 0.0f), modelViewMatrix,
                     projectionMatrix, viewport);
  return result;
}
