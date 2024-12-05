#include "game_objects.hpp"
#include <SDL_image.h>
#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

// Cria Entidades do Jogo
void GameEntities::create() {
  auto const assetsPath{
      abcg::Application::getAssetsPath()}; // Caminho dos assets

  abcg::glEnable(GL_DEPTH_TEST); // Habilita teste de profundidade

  // Configurações de luz
  m_program = abcg::createOpenGLProgram(
      {{.source = assetsPath + "shaders/texture.vert",
        .stage = abcg::ShaderStage::Vertex},
       {.source = assetsPath + "shaders/texture.frag",
        .stage = abcg::ShaderStage::Fragment}}); // Programa de shader

  createObject(m_distractionModel, assetsPath, "bird.obj",
               "maps/b-feathers.png"); // Cria objeto de distração
  createObject(m_targetModel, assetsPath, "bird.obj",
               "maps/w-feathers.png"); // Cria objeto alvo

  setupSceneObjects(m_distractionObjects, // m_sceneObjects
                    -15.0f,               // minX
                    15.0f,                // maxX
                    0.0f,                 // minY
                    15.0f,                // maxY
                    -50.0f,               // minZ
                    0.0f                  // maxZ
  );                                      // Configura objetos de distração

  setupSceneObjects(m_targetObjects, // m_sceneObjects
                    -15.0f,          // minX
                    15.0f,           // maxX
                    0.0f,            // minY
                    15.0f,           // maxY
                    -50.0f,          // minZ
                    0.0f             // maxZ
  );                                 // Configura objetos alvo
}

// Configura objetos de cena
void GameEntities::setupSceneObjects(std::vector<SceneObject> &m_sceneObjects,
                                     float minX, float maxX, float minY,
                                     float maxY, float minZ, float maxZ) {
  for (auto &sceneObject : m_sceneObjects) {
    randomizeSceneObject(sceneObject, minX, maxX, minY, maxY, minZ, maxZ);
  }
}

// Atualiza objetos de cena
void GameEntities::updateSceneObjects(std::vector<SceneObject> &m_sceneObjects,
                                      float deltaTime, float incZ, float incX,
                                      float incY, float posZ, float minX,
                                      float maxX, float minY, float maxY,
                                      float minZ, float maxZ) {

  m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f) * deltaTime); // Ângulo

  // Atualiza posição de cada objeto de cena
  for (auto &sceneObject : m_sceneObjects) {

    // Atualiza posição do objeto pelo tempo
    sceneObject.m_position.z += deltaTime * incZ;
    sceneObject.m_position.x += deltaTime * incX;
    sceneObject.m_position.y += deltaTime * incY;

    // Seleciona nova posição aleatória caso o objeto esteja fora da tela
    if (sceneObject.m_position.z > 0.1f) {
      randomizeSceneObject(sceneObject, minX, maxX, minY, maxY, minZ, maxZ);
      // Volta para a posição passada por parâmento (-100 por exemplo)
      sceneObject.m_position.z = posZ;
    }
  }
}

// Cria um objeto de cena
void GameEntities::createObject(Model &m_model, const std::string &assetsPath,
                                const std::string &objPath,
                                const std::string &texturePath) {

  // Carrega modelo, textura, tamanho e configura VAO
  m_model.loadDiffuseTexture(assetsPath + texturePath); // Textura
  m_model.loadObj(assetsPath + objPath);                // Objeto
  m_model.setSize(100.0f);                              // Tamanho
  m_model.setupVAO(m_program);                          // Configura VAO

  // Variáveis uniformes do modelo
  m_Ka = m_model.getKa();               // Coeficiente de reflexão ambiente
  m_Kd = m_model.getKd();               // Coeficiente de reflexão difusa
  m_Ks = m_model.getKs();               // Coeficiente de reflexão especular
  m_shininess = m_model.getShininess(); // Brilho

  // Camera em posição inicial
  glm::vec3 const eye{0.0f, 0.0f, 0.0f}; // Posição da câmera (origem)
  glm::vec3 const at{0.0f, 0.0f,
                     -1.0f}; // Ponto que câmera está olhando (eixo z)
  glm::vec3 const up{0.0f, 1.0f, 0.0f};    // Vetor up da câmera (eixo y)
  m_viewMatrix = glm::lookAt(eye, at, up); // Matriz de visão
}

// Aleatoriza a posição de um objeto de cena
void GameEntities::randomizeSceneObject(SceneObject &sceneObject, float minX,
                                        float maxX, float minY, float maxY,
                                        float minZ, float maxZ) {

  // Posição aleatória no espaço 3D
  std::uniform_real_distribution<float> distPosX(
      minX, maxX); // Distribuição de posição x
  std::uniform_real_distribution<float> distPosY(
      minY, maxY); // Distribuição de posição y
  std::uniform_real_distribution<float> distPosZ(
      minZ, maxZ); // Distribuição de posição z
  sceneObject.m_position =
      glm::vec3(distPosX(m_randomEngine), // Posição aleatória no eixo x
                distPosY(m_randomEngine), // Posição aleatória no eixo y
                distPosZ(m_randomEngine)  // Posição aleatória no eixo z
      );

  sceneObject.m_rotationAxis = glm::sphericalRand(1.0f); // Eixo de rotação
}

// Renderiza objetos de cena
void GameEntities::paint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa a tela

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y); // Viewport

  abcg::glUseProgram(m_program); // Usa o programa de shader

  // Atribui variáveis uniformes
  auto const viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")}; // Matriz de visão
  auto const projMatrixLoc{abcg::glGetUniformLocation(
      m_program, "projMatrix")}; // Matriz de projeção
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")}; // Matriz de modelo
  auto const normalMatrixLoc{
      abcg::glGetUniformLocation(m_program, "normalMatrix")}; // Matriz normal
  auto const lightDirLoc{
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")}; // Luz
  auto const shininessLoc{
      abcg::glGetUniformLocation(m_program, "shininess")}; // Brilho
  auto const IaLoc{
      abcg::glGetUniformLocation(m_program, "Ia")}; // Intensidade ambiente
  auto const IdLoc{
      abcg::glGetUniformLocation(m_program, "Id")}; // Intensidade difusa
  auto const IsLoc{
      abcg::glGetUniformLocation(m_program, "Is")}; // Intensidade especular
  auto const KaLoc{
      abcg::glGetUniformLocation(m_program, "Ka")}; // Coeficiente ambiente
  auto const KdLoc{
      abcg::glGetUniformLocation(m_program, "Kd")}; // Coeficiente difuso
  auto const KsLoc{
      abcg::glGetUniformLocation(m_program, "Ks")}; // Coeficiente especular
  auto const diffuseTexLoc{
      abcg::glGetUniformLocation(m_program, "diffuseTex")}; // Textura
  auto const lightDirRotated{
      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}; // Luz rotacionada

  // Aplica variáveis uniformes
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_viewMatrix[0][0]); // Matriz de visão
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_projMatrix[0][0]); // Matriz de projeção
  abcg::glUniform1i(diffuseTexLoc, 0);           // Textura
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x); // Luz
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x); // Intensidade ambiente
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x); // Intensidade difusa
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x); // Intensidade especular

  // Renderiza objetos de cena
  renderObject(m_distractionModel, KaLoc, KdLoc, KsLoc, shininessLoc,
               modelMatrixLoc, m_distractionObjects);
  renderObject(m_targetModel, KaLoc, KdLoc, KsLoc, shininessLoc, modelMatrixLoc,
               m_targetObjects);

  // Finaliza
  abcg::glUseProgram(0);
}

// TODO: Renderizar solo/chão adequadamente
// Renderiza objetos de cena
void GameEntities::renderObject(Model &m_model, const GLint KaLoc,
                                const GLint KdLoc, const GLint KsLoc,
                                const GLint shininessLoc,
                                const GLint modelMatrixLoc,
                                std::vector<SceneObject> &m_scObjects) {

  // Aplica variáveis uniformes para o modelo de matriz de protótipo
  glm::mat4 modelMatrixProt{1.0f}; // Matriz de modelo protótipo
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE,
                           &modelMatrixProt[0][0]); // Matriz de modelo
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);            // Coeficiente ambiente
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);            // Coeficiente difuso
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);            // Coeficiente especular
  abcg::glUniform1f(shininessLoc, m_shininess);     // Brilho

  // Renderiza cada objeto de cena
  for (auto &scObject : m_scObjects) {

    // Computa a matriz de modelo para todos objeto de cena
    glm::mat4 modelMatrix{1.0f}; // Matriz de modelo
    modelMatrix =
        glm::translate(modelMatrix, scObject.m_position); // Posição do objeto
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7f)); // Escala do objeto
    modelMatrix = glm::rotate(modelMatrix, m_angle,
                              scObject.m_rotationAxis); // Rotação do objeto

    // Aplica variáveis uniformes para o modelo
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    m_model.render();
  }
}

// Verifica se um objeto de cena foi clicado
bool GameEntities::checkClickOnObject(
    glm::vec3 const &clickPos, glm::mat4 const &viewMatrix,
    glm::mat4 const &projMatrix, std::vector<SceneObject> &m_sceneObjects) {

  // Verifica se o clique atingiu um objeto de cena
  for (auto &sceneObject : m_sceneObjects) {

    glm::vec3 sceneObjectPosition =
        sceneObject.m_position; // Posição 3D do objeto de cena (pássaro)

    // Converte a posição do objeto de cena para o espaço da tela
    glm::vec4 sceneObjectClipSpace =
        projMatrix * viewMatrix * glm::vec4(sceneObjectPosition, 1.0f);
    // Normaliza no espaço de recorte
    sceneObjectClipSpace /= sceneObjectClipSpace.w;

    // A posição do objeto que está em -1 a 1 é escalada para o tamanho da tela
    glm::vec2 sceneObjectScreenPos = {
        (sceneObjectClipSpace.x * 0.5f + 0.5f) * m_viewportSize.x,
        (1.0f - (sceneObjectClipSpace.y * 0.5f + 0.5f)) * m_viewportSize.y};

    // Distância no espaço da tela (clique projetado em 2D)
    float halfSize = 100.0f; // Tamanho do objeto em pixels na tela
    if (glm::distance(glm::vec3(clickPos.x, clickPos.y, clickPos.z),
                      glm::vec3(sceneObjectScreenPos, 0.0f)) <= halfSize) {
      // TODO: Remover objeto clicado
      // Reposiciona objeto clicada
      randomizeSceneObject(sceneObject, // sceneObject
                           -15.0f,      // minX
                           15.0f,       // maxX
                           0.0f,        // minY
                           15.0f,       // maxY
                           -50.0f,      // minZ
                           0.0f         // maxZ
      );
      // Clique reconhecido
      return true;
    }
  }
  // Nenhum objeto na cena foi clicado
  return false;
}

// Atualiza objetos de cena
void GameEntities::update(float deltaTime) {
  // Atualiza objetos de cena de distração
  updateSceneObjects(m_distractionObjects, // m_sceneObjects
                     deltaTime,            // deltaTime
                     15.0f,                // incZ
                     -1.0f,                // incX
                     0.0f,                 // incY
                     -50.0f,               // posZ
                     -15.0f,               // minX
                     15.0f,                // maxX
                     0.0f,                 // minY
                     25.0f,                // maxY
                     -100.0f,              // minZ
                     0.0f                  // maxZ
  );
  // Atualiza objetos de cena de alvo
  updateSceneObjects(m_targetObjects, // m_sceneObjects
                     deltaTime,       // deltaTime
                     10.0f,           // incZ
                     0.0f,            // incX
                     0.0f,            // incY
                     -50.0f,          // posZ
                     -15.0f,          // minX
                     15.0f,           // maxX
                     0.0f,            // minY
                     15.0f,           // maxY
                     -50.0f,          // minZ
                     0.0f             // maxZ
  );
}

// Renderiza UI
// TODO: Ter uma matriz de projeção diferente para o cenário 3D
void GameEntities::paintUI() {
  {
    auto const widgetSize{ImVec2(218, 62)}; // Variável do tamanho do widget
    ImGui::SetNextWindowPos(
        ImVec2(m_viewportSize.x - widgetSize.x - 5, 5)); // Posição do widget
    ImGui::SetNextWindowSize(widgetSize);                // Tamanho do widget
    // Cria janela do widget
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);
    {
      ImGui::PushItemWidth(120);         // Largura do item
      static std::size_t currentIndex{}; // Índice atual
      // Itens do combo
      std::vector<std::string> const comboItems{"Treinamento",
                                                "Vôo dos Pássaros"};
      // Cria combo
      if (ImGui::BeginCombo("Projeção", comboItems.at(currentIndex).c_str())) {
        for (auto const index : iter::range(comboItems.size())) {
          auto const isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo(); // Finaliza combo
      }
      ImGui::PopItemWidth(); // Finaliza largura do item

      // Configurações do widget
      ImGui::PushItemWidth(170);
      auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                        gsl::narrow<float>(m_viewportSize.y)};
      if (currentIndex == 0) {
        m_projMatrix =
            glm::perspective(glm::radians(m_FOV), aspect, 0.01f,
                             100.0f); // Matriz de projeção de perspectiva

        ImGui::SliderFloat("FOV", &m_FOV, 5.0f, 179.0f, "%.0f degrees");
      } else {
        m_projMatrix =
            glm::ortho(-20.0f * aspect, 20.0f * aspect, -20.0f, 20.0f, 0.01f,
                       100.0f); // Matriz de projeção ortográfica
      }
      ImGui::PopItemWidth(); // Finaliza configurações do widget
    }
    ImGui::End(); // Finaliza janela do widget
  }
}

// Finaliza objetos de cena
void GameEntities::destroy() {
  m_distractionModel.destroy();
  m_targetModel.destroy();
  abcg::glDeleteProgram(m_program);
}