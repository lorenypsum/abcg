#include "game_objects.hpp"
#include <SDL_image.h>
#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

// Cria objetos
void GameEntities::create() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(1, 0.753, 0.961, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "texture.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "texture.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  createObject(m_distractionModel, assetsPath, "Grass_Block.obj",
               "maps/Grass.png");
  createObject(m_targetModel, assetsPath, "Grass_Block.obj", "maps/a.png");

  setupSceneObjects(m_distractionObjects, // m_sceneObjects
                    -25.0f,               // minX
                    25.0f,                // maxX
                    -25.0f,               // minY
                    25.0f,                // maxY
                    -100.0f,              // minZ
                    0.0f                  // maxZ
  );
  setupSceneObjects(m_targetObjects, // m_sceneObjects
                    -25.0f,          // minX
                    25.0f,           // maxX
                    -25.0f,          // minY
                    25.0f,           // maxY
                    -100.0f,         // minZ
                    0.0f             // maxZ
  );
}

void GameEntities::setupSceneObjects(std::vector<SceneObject> &m_sceneObjects,
                                     float minX, float maxX, float minY,
                                     float maxY, float minZ, float maxZ) {
  // Setup scene objects
  for (auto &sceneObject : m_sceneObjects) {
    randomizeSceneObject(sceneObject, minX, maxX, minY, maxY, minZ, maxZ);
  }
}

void GameEntities::updateSceneObjects(std::vector<SceneObject> &m_sceneObjects,
                                      float deltaTime, float incZ, float incX,
                                      float incY, float posZ, float minX,
                                      float maxX, float minY, float maxY,
                                      float minZ, float maxZ) {

  m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f) * deltaTime);

  // Update scene objects
  for (auto &sceneObject : m_sceneObjects) {
    // Increase z by incZ units per second
    sceneObject.m_position.z += deltaTime * incZ;
    sceneObject.m_position.x += deltaTime * incX;
    sceneObject.m_position.y += deltaTime * incY;

    // If this sceneObject is behind the camera, select a new random position
    if (sceneObject.m_position.z > 0.1f) {
      randomizeSceneObject(sceneObject, minX, maxX, minY, maxY, minZ, maxZ);
      sceneObject.m_position.z = posZ; // Back to -100
    }
  }
}

void GameEntities::createObject(Model &m_model, const std::string &assetsPath,
                                const std::string &objPath,
                                const std::string &texturePath) {
  m_model.loadDiffuseTexture(assetsPath + texturePath);
  m_model.loadObj(assetsPath + objPath);
  m_model.setSize(10.0f);
  m_model.setupVAO(m_program);

  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();

  // Camera at (0,0,0) and looking towards the negative z
  glm::vec3 const eye{0.0f, 0.0f, 0.0f};
  glm::vec3 const at{0.0f, 0.0f, -1.0f};
  glm::vec3 const up{0.0f, 1.0f, 0.0f};
  m_viewMatrix = glm::lookAt(eye, at, up);
}

void GameEntities::randomizeSceneObject(SceneObject &sceneObject, float minX,
                                        float maxX, float minY, float maxY,
                                        float minZ, float maxZ) {
  // Random position: x and y in [minX, maxX), z in [minZ, maxZ)
  std::uniform_real_distribution<float> distPosX(minX, maxX);
  std::uniform_real_distribution<float> distPosY(minY, maxY);
  std::uniform_real_distribution<float> distPosZ(minZ, maxZ);
  sceneObject.m_position =
      glm::vec3(distPosX(m_randomEngine), distPosY(m_randomEngine),
                distPosZ(m_randomEngine));

  // Random rotation axis
  sceneObject.m_rotationAxis = glm::sphericalRand(1.0f);
}

void GameEntities::paint() {
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

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);

  auto const lightDirRotated{glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)};

  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  renderObject(m_distractionModel, KaLoc, KdLoc, KsLoc, shininessLoc,
               modelMatrixLoc, m_distractionObjects);
  renderObject(m_targetModel, KaLoc, KdLoc, KsLoc, shininessLoc, modelMatrixLoc,
               m_targetObjects);

  abcg::glUseProgram(0);
}

void GameEntities::renderObject(Model &m_model, const GLint KaLoc,
                                const GLint KdLoc, const GLint KsLoc,
                                const GLint shininessLoc,
                                const GLint modelMatrixLoc,
                                std::vector<SceneObject> &m_sceneObjects) {
  // Set uniform variables for the current model
  glm::mat4 modelMatrixProt{1.0f};

  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  // Set uniform variable
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrixProt[0][0]);

  // Render each sceneObject
  for (auto &sceneObject : m_sceneObjects) {
    // Compute model matrix of the current sceneObject
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, sceneObject.m_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, sceneObject.m_rotationAxis);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    m_model.render();
  }
}

bool GameEntities::checkClickOnObject(
    glm::vec3 const &clickPos, glm::mat4 const &viewMatrix,
    glm::mat4 const &projMatrix, std::vector<SceneObject> &m_sceneObjects) {
  for (auto &sceneObject : m_sceneObjects) {
    // Posição 3D da estrela
    glm::vec3 sceneObjectPosition = sceneObject.m_position;

    // Converta a posição do objeto de cena para o espaço da tela
    glm::vec4 sceneObjectClipSpace =
        projMatrix * viewMatrix * glm::vec4(sceneObjectPosition, 1.0f);
    // Normaliza no espaço de recorte
    sceneObjectClipSpace /= sceneObjectClipSpace.w;

    // A posição da estrela está em -1 a 1.
    // Precisamos escalá-la para coordenadas da janela
    glm::vec2 sceneObjectScreenPos = {
        (sceneObjectClipSpace.x * 0.5f + 0.5f) * m_viewportSize.x,
        (1.0f - (sceneObjectClipSpace.y * 0.5f + 0.5f)) * m_viewportSize.y};

    // Distância no espaço da tela (clique projetado em 2D)
    float halfSize = 50.0f; // Tamanho do objeto em pixels na tela
    if (glm::distance(glm::vec3(clickPos.x, clickPos.y, clickPos.z),
                      glm::vec3(sceneObjectScreenPos, 0.0f)) <= halfSize) {
      // Reposiciona estrela clicada
      randomizeSceneObject(sceneObject, -25.0f, 25.0f, -25.0f, 25.0f, -100.0f,
                           0.0f);
      // Clique reconhecido
      return true;
    }
  }
  // Nenhum objeto na cena foi clicado
  return false;
}

void GameEntities::update(float deltaTime) {
  // Increase angle by 90 degrees per second
  updateSceneObjects(m_distractionObjects, // m_sceneObjects
                     deltaTime,            // deltaTime
                     10.0f,                // incZ
                     -2.0f,                 // incX
                     0.0f,                 // incY
                     -50.0f,               // posZ
                     -25.0f,               // minX
                     25.0f,                // maxX
                     -25.0f,               // minY
                     25.0f,                // maxY
                     -100.0f,              // minZ
                     0.0f                  // maxZ
  );

  updateSceneObjects(m_targetObjects, // m_sceneObjects
                     deltaTime,       // deltaTime
                     10.0f,           // incZ
                     2.0f,            // incX
                     0.0f,            // incY
                     -50.0f,          // posZ
                     -25.0f,          // minX
                     25.0f,           // maxX
                     -25.0f,          // minY
                     25.0f,           // maxY
                     -100.0f,         // minZ
                     0.0f             // maxZ
  );
}

void GameEntities::destroy() {
  m_distractionModel.destroy();
  m_targetModel.destroy();
  abcg::glDeleteProgram(m_program);
}

void GameEntities::paintUI() {
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