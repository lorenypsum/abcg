#include "game_objects.hpp"
#include "imfilebrowser.h"
#include <SDL_image.h>
#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <stdio.h>

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

  // Configurações de luz
  m_program_2 = abcg::createOpenGLProgram(
      {{.source = assetsPath + "shaders/normalmapping.vert",
        .stage = abcg::ShaderStage::Vertex},
       {.source = assetsPath + "shaders/normalmapping.frag",
        .stage = abcg::ShaderStage::Fragment}}); // Programa de shader

  // Create skybox program
  // Configurações de luz
  m_skyProgram = abcg::createOpenGLProgram(
      {{.source = assetsPath + "shaders/skybox.vert",
        .stage = abcg::ShaderStage::Vertex},
       {.source = assetsPath + "shaders/skybox.frag",
        .stage = abcg::ShaderStage::Fragment}}); // Programa de shader

  // Camera em posição inicial
  glm::vec3 const eye{0.0f, 0.0f, 0.0f}; // Posição da câmera (origem)
  glm::vec3 const at{0.0f, 0.0f, -1.0f}; // Olhar da câmera (eixo z)
  glm::vec3 const up{0.0f, 1.0f, 0.0f};  // Vetor up da câmera (eixo y)

  createSkybox(m_skyProgram); // Cria skybox
  createObject(m_program, m_distractionModel, assetsPath, "bird.obj",
               "maps/pink.jpg",
               "maps/texture.jpg"); // Cria objeto de distração
  createObject(m_program, m_distractionModel1, assetsPath, "bird.obj",
               "maps/beige.jpg",
               "maps/texture.jpg"); // Cria objeto de distração
  createObject(m_program, m_distractionModel2, assetsPath, "bird.obj",
               "maps/red.jpg",
               "maps/texture.jpg"); // Cria objeto de distração
  createObject(m_program, m_targetModel, assetsPath, "bird.obj",
               "maps/white.jpg", "maps/texture.jpg"); // Cria objeto alvo
  createObject(m_program_2, m_netModel, assetsPath, "wicker_basket.obj",
               "maps/wood.jpg", "maps/normal.jpg"); // Cria objeto alvo

  float x = 0.0f;
  float y = -0.3f; // Mantém Y fixo
  float z = -2.0f; // Mantém Z fixo

  m_net.m_position = glm::vec3(x, y, z);
  m_net.m_rotationAxis = glm::vec3(0.1f, 0.0f, 0.0f);

  setupSceneObjects(m_distractionObjects, // m_sceneObjects
                    -15.0f,               // minX
                    15.0f,                // maxX
                    -15.0f,               // minY
                    15.0f,                // maxY
                    -50.0f,               // minZ
                    0.0f                  // maxZ
  );                                      // Configura objetos de distração

  setupSceneObjects(m_distractionObjects1, // m_sceneObjects
                    -15.0f,                // minX
                    15.0f,                 // maxX
                    -15.0f,                // minY
                    15.0f,                 // maxY
                    -50.0f,                // minZ
                    0.0f                   // maxZ
  );                                       // Configura objetos de distração

  setupSceneObjects(m_distractionObjects2, // m_sceneObjects
                    -15.0f,                // minX
                    15.0f,                 // maxX
                    -15.0f,                // minY
                    15.0f,                 // maxY
                    -50.0f,                // minZ
                    0.0f                   // maxZ
  );                                       // Configura objetos de distração

  setupSceneObjects(m_targetObjects, // m_sceneObjects
                    -15.0f,          // minX
                    15.0f,           // maxX
                    -5.0f,           // minY
                    15.0f,           // maxY
                    -50.0f,          // minZ
                    0.0f             // maxZ
  );                                 // Configura objetos alvo
}

// Atualiza objetos de cena
void GameEntities::update(float deltaTime) {
  // Atualiza objetos de cena de distração
  updateSceneObjects(m_distractionObjects, // m_sceneObjects
                     deltaTime,            // deltaTime
                     15.0f,                // incZ
                     0.0f,                 // incX
                     1.0f,                 // incY
                     -50.0f,               // posZ
                     -15.0f,               // minX
                     15.0f,                // maxX
                     -5.0f,                // minY
                     25.0f,                // maxY
                     -100.0f,              // minZ
                     0.0f                  // maxZ
  );
  updateSceneObjects(m_distractionObjects1, // m_sceneObjects
                     deltaTime,             // deltaTime
                     15.0f,                 // incZ
                     2.0f,                  // incX
                     1.0f,                  // incY
                     -50.0f,                // posZ
                     -15.0f,                // minX
                     15.0f,                 // maxX
                     -5.0f,                 // minY
                     25.0f,                 // maxY
                     -100.0f,               // minZ
                     0.0f                   // maxZ
  );
  updateSceneObjects(m_distractionObjects2, // m_sceneObjects
                     deltaTime,             // deltaTime
                     15.0f,                 // incZ
                     -2.0f,                  // incX
                     1.0f,                  // incY
                     -50.0f,                // posZ
                     -15.0f,                // minX
                     15.0f,                 // maxX
                     -5.0f,                 // minY
                     25.0f,                 // maxY
                     -100.0f,               // minZ
                     0.0f                   // maxZ
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
                     -5.0f,           // minY
                     15.0f,           // maxY
                     -50.0f,          // minZ
                     0.0f             // maxZ
  );
}

void GameEntities::createSkybox(GLuint &program) {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  m_skyModel.loadCubeTexture(assetsPath + "maps/cube/");

  // Generate VBO
  abcg::glGenBuffers(1, &m_skyVBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyPositions),
                     m_skyPositions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_skyVAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_skyVAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_TRUE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

// Cria um objeto de cena
void GameEntities::createObject(GLuint &program, Model &m_model,
                                const std::string &assetsPath,
                                const std::string &objPath,
                                const std::string &texturePath,
                                const std::string &normalPath) {

  // Carrega modelo, textura, tamanho e configura VAO
  m_model.loadDiffuseTexture(assetsPath + texturePath); // Textura
  m_model.loadNormalTexture(assetsPath + normalPath);   // Textura Normal
  m_model.loadObj(assetsPath + objPath);                // Objeto
  m_model.setSize(100.0f);                              // Tamanho
  m_model.setupVAO(program);                            // Configura VAO
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

  // Renderiza objetos de cena
  renderSkybox(m_skyProgram);
  renderObject(m_program, m_distractionModel, m_distractionObjects);
  renderObject(m_program, m_distractionModel1, m_distractionObjects1);
  renderObject(m_program, m_distractionModel2, m_distractionObjects2);
  renderObject(m_program, m_targetModel, m_targetObjects);
  renderNet(m_program_2, m_netModel, m_netObjects);

  // Finaliza
  abcg::glUseProgram(0);
}

// TODO: Renderizar solo/chão adequadamente
// Renderiza objetos de cena
void GameEntities::renderObject(GLuint &program, Model &m_model,
                                std::vector<SceneObject> &m_scObjects) {
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y); // Viewport

  abcg::glUseProgram(program); // Usa o programa de shader
  // Variáveis uniformes do modelo
  m_Ka = m_model.getKa();               // Coeficiente de reflexão ambiente
  m_Kd = m_model.getKd();               // Coeficiente de reflexão difusa
  m_Ks = m_model.getKs();               // Coeficiente de reflexão especular
  m_shininess = m_model.getShininess(); // Brilho

  // Atribui variáveis uniformes
  auto const viewMatrixLoc{
      abcg::glGetUniformLocation(program, "viewMatrix")}; // Matriz de visão
  auto const projMatrixLoc{
      abcg::glGetUniformLocation(program, "projMatrix")}; // Matriz de projeção
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")}; // Matriz de modelo
  auto const lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")}; // Luz
  auto const shininessLoc{
      abcg::glGetUniformLocation(program, "shininess")}; // Brilho
  auto const IaLoc{
      abcg::glGetUniformLocation(program, "Ia")}; // Intensidade ambiente
  auto const IdLoc{
      abcg::glGetUniformLocation(program, "Id")}; // Intensidade difusa
  auto const IsLoc{
      abcg::glGetUniformLocation(program, "Is")}; // Intensidade especular
  auto const KaLoc{
      abcg::glGetUniformLocation(program, "Ka")}; // Coeficiente ambiente
  auto const KdLoc{
      abcg::glGetUniformLocation(program, "Kd")}; // Coeficiente difuso
  auto const KsLoc{
      abcg::glGetUniformLocation(program, "Ks")}; // Coeficiente especular
  auto const diffuseTexLoc{
      abcg::glGetUniformLocation(program, "diffuseTex")}; // Textura
  auto const normalTexLoc{abcg::glGetUniformLocation(program, "normalTex")};
  auto const lightDirRotated{
      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}; // Luz rotacionada

  // Aplica variáveis uniformes
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_viewMatrix[0][0]); // Matriz de visão
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_projMatrix[0][0]); // Matriz de projeção
  abcg::glUniform1i(diffuseTexLoc, 0);           // Textura
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x); // Luz
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x); // Intensidade ambiente
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x); // Intensidade difusa
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x); // Intensidade especular

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

// Renderiza rede da cena
void GameEntities::renderNet(GLuint &program, Model &m_model,
                             std::vector<SceneObject> &m_scObjects) {
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y); // Viewport

  abcg::glUseProgram(program); // Usa o programa de shader

  m_Ka = m_model.getKa();               // Coeficiente de reflexão ambiente
  m_Kd = m_model.getKd();               // Coeficiente de reflexão difusa
  m_Ks = m_model.getKs();               // Coeficiente de reflexão especular
  m_shininess = m_model.getShininess(); // Brilho

  // Atribui variáveis uniformes
  auto const viewMatrixLoc{
      abcg::glGetUniformLocation(program, "viewMatrix")}; // Matriz de visão
  auto const projMatrixLoc{
      abcg::glGetUniformLocation(program, "projMatrix")}; // Matriz de projeção
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")}; // Matriz de modelo
  auto const lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")}; // Luz
  auto const shininessLoc{
      abcg::glGetUniformLocation(program, "shininess")}; // Brilho
  auto const IaLoc{
      abcg::glGetUniformLocation(program, "Ia")}; // Intensidade ambiente
  auto const IdLoc{
      abcg::glGetUniformLocation(program, "Id")}; // Intensidade difusa
  auto const IsLoc{
      abcg::glGetUniformLocation(program, "Is")}; // Intensidade especular
  auto const KaLoc{
      abcg::glGetUniformLocation(program, "Ka")}; // Coeficiente ambiente
  auto const KdLoc{
      abcg::glGetUniformLocation(program, "Kd")}; // Coeficiente difuso
  auto const KsLoc{
      abcg::glGetUniformLocation(program, "Ks")}; // Coeficiente especular
  auto const diffuseTexLoc{
      abcg::glGetUniformLocation(program, "diffuseTex")}; // Textura
  auto const normalTexLoc{abcg::glGetUniformLocation(program, "normalTex")};
  auto const lightDirRotated{
      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}; // Luz rotacionada

  // Aplica variáveis uniformes
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_viewMatrix[0][0]); // Matriz de visão
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_projMatrix[0][0]); // Matriz de projeção
  abcg::glUniform1i(diffuseTexLoc, 0);           // Textura
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x); // Luz
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x); // Intensidade ambiente
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x); // Intensidade difusa
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x); // Intensidade especular

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
    modelMatrix = glm::translate(modelMatrix, m_net.m_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f)); // Ajusta o tamanho
    modelMatrix = glm::rotate(modelMatrix, 11.0f, m_net.m_rotationAxis);

    // Aplica variáveis uniformes para o modelo
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    m_model.render();
  }
}

void GameEntities::renderSkybox(GLuint &program) {
  abcg::glUseProgram(program);

  auto const viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  auto const skyTexLoc{abcg::glGetUniformLocation(program, "skyTex")};

  auto const viewMatrix{m_trackBallLight.getRotation()};
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(skyTexLoc, 0);

  abcg::glBindVertexArray(m_skyVAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyModel.getCubeTexture());

  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);
  abcg::glDrawArrays(GL_TRIANGLES, 0, m_skyPositions.size());
  abcg::glDepthFunc(GL_LESS);
  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void GameEntities::destroySkybox(GLuint &program) const {
  abcg::glDeleteProgram(program);
  abcg::glDeleteBuffers(1, &m_skyVBO);
  abcg::glDeleteVertexArrays(1, &m_skyVAO);
}

bool GameEntities::checkCollisionWithNet(
    glm::vec3 const &netPosition, float netRadius,
    std::vector<SceneObject> &m_sceneObjects, float objectRadius) {
  // Para esse caso, netRadius pode ser visto como a "largura" da rede na
  // direção X, Y, Z Como você tem formas não esféricas, vamos usar uma
  // abordagem AABB

  for (auto &sceneObject : m_sceneObjects) {
    glm::vec3 sceneObjectPosition = sceneObject.m_position;

    // Definir a caixa delimitadora para o objeto de cena, baseando-se no centro
    // e no raio
    glm::vec3 minBound = sceneObjectPosition - glm::vec3(objectRadius);
    glm::vec3 maxBound = sceneObjectPosition + glm::vec3(objectRadius);

    // Definir a caixa delimitadora para a rede (semelhante)
    glm::vec3 netMinBound = netPosition - glm::vec3(netRadius);
    glm::vec3 netMaxBound = netPosition + glm::vec3(netRadius);

    // Verificar se há sobreposição entre as caixas AABB
    if (minBound.x <= netMaxBound.x && maxBound.x >= netMinBound.x &&
        minBound.y <= netMaxBound.y && maxBound.y >= netMinBound.y &&
        minBound.z <= netMaxBound.z && maxBound.z >= netMinBound.z) {
      // Se as caixas AABB se sobrepõem, há colisão
      return true;
    }
  }

  // Nenhuma colisão detectada
  return false;
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
      std::vector<std::string> const comboItems{"Vôo dos Pássaros",
                                                "Cenário 3D"}; // Itens do combo
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
            glm::perspective(glm::radians(m_FOV), aspect, 0.05f,
                             100.0f); // Matriz de projeção de perspectiva
        ImGui::SliderFloat("Size", &m_viewMatrix[3][2], -10.0f, 10.0f);
      } else {
        m_projMatrix =
            glm::perspective(glm::radians(m_FOV), aspect, 0.01f,
                             100.0f); // Matriz de projeção de perspectiv
        ImGui::SliderFloat("View", &m_FOV, 5.0f, 179.0f, "%.0f degrees");
      }
      ImGui::PopItemWidth(); // Finaliza configurações do widget
    }
    ImGui::End(); // Finaliza janela do widget
  }
}

// Finaliza objetos de cena
void GameEntities::destroy() {
  m_distractionModel.destroy();
  m_distractionModel1.destroy();
  m_distractionModel2.destroy();
  m_targetModel.destroy();
  m_netModel.destroy();
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteProgram(m_program_2);
}