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
      abcg::createOpenGLProgram({{.source = assetsPath + "texture.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "texture.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_distraction.loadDiffuseTexture(assetsPath + "maps/Grass.png");
  m_distraction.loadObj(assetsPath + "Grass_Block.obj");
  m_distraction.setSize(10.0f);
  m_distraction.setupVAO(m_program);

  m_target.loadDiffuseTexture(assetsPath + "maps/a.png");
  m_target.loadObj(assetsPath + "teapot.obj");
  m_target.setSize(10.0f);
  m_target.setupVAO(m_program);

  m_Ka = m_distraction.getKa();
  m_Kd = m_distraction.getKd();
  m_Ks = m_distraction.getKs();
  m_shininess = m_distraction.getShininess();

  // Camera at (0,0,0) and looking towards the negative z
  glm::vec3 const eye{0.0f, 0.0f, 0.0f};
  glm::vec3 const at{0.0f, 0.0f, -1.0f};
  glm::vec3 const up{0.0f, 1.0f, 0.0f};
  m_viewMatrix = glm::lookAt(eye, at, up);

  // Setup stars
  for (auto &star : m_stars) {
    randomizeStar(star);
  }

    // Setup starsx
  for (auto &starx : m_starsx) {
    randomizeStarx(starx);
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

void DistractionObjects::randomizeStarx(Starx &starx) {
  // Random position: x and y in [-20, 20), z in [-100, 0)
  std::uniform_real_distribution<float> distPosXY(-25.0f, 25.0f);
  std::uniform_real_distribution<float> distPosZ(-100.0f, 0.0f);
  starx.m_position =
      glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                distPosZ(m_randomEngine));

  // Random rotation axis
  starx.m_rotationAxis = glm::sphericalRand(1.5f);
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
  auto const normalMatrixLoc{
      abcg::glGetUniformLocation(m_program, "normalMatrix")};
  auto const lightDirLoc{
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};
  auto const shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};

  auto const IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  auto const KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  auto const diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};

  // // Set uniform variables that have the same value for every model
  // abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
  //                          &m_camera.getViewMatrix()[0][0]);
  // abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
  //                          &m_camera.getProjMatrix()[0][0]);

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);

  auto const lightDirRotated{glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)};

  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables for the current model
  glm::mat4 modelMatrixProt{1.0f};

  // auto const modelViewMatrix{
  //     glm::mat3(m_camera.getViewMatrix() * modelMatrixProt)};
  // auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  // abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
  // &normalMatrix[0][0]);

  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  // modelMatrixProt = glm::translate(modelMatrixProt, m_position);
  // modelMatrixProt = glm::rotate(modelMatrixProt, m_angle, glm::vec3(0, 1,
  // 0)); modelMatrixProt = glm::scale(modelMatrixProt, glm::vec3(0.15f));

  // Set uniform variable
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrixProt[0][0]);

  // Render each star
  for (auto &star : m_stars) {
    // Compute model matrix of the current star
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, star.m_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, star.m_rotationAxis);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    m_distraction.render();
  }

    // Render pot
  for (auto &starx : m_starsx) {
    // Compute model matrix of the current star
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, starx.m_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, starx.m_rotationAxis);
  
    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
  
    m_target.render();
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
    float halfSize = 50.0f; // Tamanho do objeto em pixels na tela
    if (glm::distance(glm::vec2(clickPos.x, clickPos.y), starScreenPos) <=
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

    // Update stars
  for (auto &starx : m_starsx) {
    // Increase z by 10 units per second
    starx.m_position.z += deltaTime * 10.0f;

    // If this star is behind the camera, select a new random position &
    // orientation and move it back to -100
    if (starx.m_position.z > 0.1f) {
      randomizeStarx(starx);
      starx.m_position.z = -50.0f; // Back to -100
    }
  }
}

void DistractionObjects::destroy() {
  m_distraction.destroy();
  m_target.destroy();
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