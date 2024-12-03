#include "distraction_objects.hpp"
#include <SDL_image.h>
#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

// Cria objetos de distração
void DistractionObjects::create() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(1, 0.753, 0.961, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "depth.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "depth.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_model.loadObj(assetsPath + "box.obj");
  m_model.loadDiffuseTexture(assetsPath + "box.png");
  m_model.setSize(10.0f);
  m_model.setupVAO(m_program);

  // Camera at (0,0,0) and looking towards the negative z
  glm::vec3 const eye{0.0f, 0.0f, 0.0f};
  glm::vec3 const at{0.0f, 0.0f, -1.0f};
  glm::vec3 const up{0.0f, 1.0f, 0.0f};
  m_viewMatrix = glm::lookAt(eye, at, up);

  // Setup stars
  for (auto &star : m_stars) {
    randomizeStar(star);
  }
}

void DistractionObjects::randomizeStar(Star &star) {
  // Random position: x and y in [-20, 20), z in [-100, 0)
  std::uniform_real_distribution<float> distPosXY(-20.0f, 20.0f);
  std::uniform_real_distribution<float> distPosZ(-50.0f, 0.0f);
  star.m_position =
      glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                distPosZ(m_randomEngine));

  // Random rotation axis
  star.m_rotationAxis = glm::sphericalRand(1.0f);
}

void DistractionObjects::paint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables
  auto const viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  auto const colorLoc{abcg::glGetUniformLocation(m_program, "color")};

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // White

  // Render each star
  for (auto &star : m_stars) {
    // Compute model matrix of the current star
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, star.m_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, star.m_rotationAxis);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    m_model.render();
  }

  abcg::glUseProgram(0);
}

bool DistractionObjects::checkClickOnDistraction(glm::vec3 const &clickPos,
                                                 glm::mat4 const &viewMatrix,
                                                 glm::mat4 const &projMatrix) {
  for (auto &star : m_stars) {
    // Posição 3D da estrela
    glm::vec3 starPosition = star.m_position;

    // Converta o starPosition para o espaço da tela
    glm::vec4 starClipSpace =
        projMatrix * viewMatrix * glm::vec4(starPosition, 1.0f);
    starClipSpace /= starClipSpace.w; // Normaliza no espaço de recorte

    // A posição da estrela está em -1 a 1. Precisamos escalá-la para
    // coordenadas da janela
    glm::vec2 starScreenPos = {
        (starClipSpace.x * 0.5f + 0.5f) * m_viewportSize.x,
        (1.0f - (starClipSpace.y * 0.5f + 0.5f)) * m_viewportSize.y};

    // Distância no espaço da tela (clique projetado em 2D)
    float halfSize = 30.0f; // Tamanho do objeto em pixels na tela
    if (glm::distance(glm::vec2(clickPos.x, clickPos.y), starScreenPos) <
        halfSize) {
      randomizeStar(star); // Reposiciona estrela clicada
      return true;         // Clique reconhecido
    }
  }
  return false; // Nenhuma estrela foi clicada
}

void DistractionObjects::update(float deltaTime) {
  // Increase angle by 90 degrees per second
  m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f) * deltaTime);

  // Update stars
  for (auto &star : m_stars) {
    // Increase z by 10 units per second
    star.m_position.z += deltaTime * 10.0f;

    // If this star is behind the camera, select a new random position &
    // orientation and move it back to -100
    if (star.m_position.z > 0.1f) {
      randomizeStar(star);
      star.m_position.z = -50.0f; // Back to -100
    }
  }
}

void DistractionObjects::destroy() {
  m_model.destroy();
  abcg::glDeleteProgram(m_program);
}

void DistractionObjects::paintUI() {
  {
    auto const widgetSize{ImVec2(218, 62)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);

    {
      ImGui::PushItemWidth(120);
      static std::size_t currentIndex{};
      std::vector<std::string> const comboItems{"Perspective", "Orthographic"};

      if (ImGui::BeginCombo("Projection",
                            comboItems.at(currentIndex).c_str())) {
        for (auto const index : iter::range(comboItems.size())) {
          auto const isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      ImGui::PushItemWidth(170);
      auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                        gsl::narrow<float>(m_viewportSize.y)};
      if (currentIndex == 0) {
        m_projMatrix =
            glm::perspective(glm::radians(m_FOV), aspect, 0.01f, 100.0f);

        ImGui::SliderFloat("FOV", &m_FOV, 5.0f, 179.0f, "%.0f degrees");
      } else {
        m_projMatrix = glm::ortho(-20.0f * aspect, 20.0f * aspect, -20.0f,
                                  20.0f, 0.01f, 100.0f);
      }
      ImGui::PopItemWidth();
    }

    ImGui::End();
  }
}