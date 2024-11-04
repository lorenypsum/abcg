#include "background.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

void Background::create() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Compila e linka o shader para o fundo
  m_program = abcg::createOpenGLProgram({
      {.source = assetsPath + "/shaders/simple.vert",
       .stage = abcg::ShaderStage::Vertex},
      {.source = assetsPath + "/shaders/simple.frag",
       .stage = abcg::ShaderStage::Fragment},
  });

  if (m_program == 0) {
    throw abcg::RuntimeError(
        "Não foi possível criar o programa do shader para o cenário.");
  }
}

void Background::paint() const {
  glUseProgram(m_program);

  // Desenha o céu
  drawSky();

  // Desenha o chão
  drawGround();

  // Desenha montanhas no cenário
  drawMountain(-0.8f, -0.2f, 0.4f);
  drawMountain(0.0f, -0.2f, 0.6f);
  drawMountain(0.7f, -0.2f, 0.5f);

  glUseProgram(0); // Desativa o programa do shader após desenhar
}

void Background::drawSky() const {
  std::vector<glm::vec2> skyVertices = {
      {-1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f}, {1.0f, 0.0f}};
  std::vector<glm::vec3> skyColors = {{0.5f, 0.8f, 1.0f},
                                      {0.5f, 0.8f, 1.0f},
                                      {0.5f, 0.8f, 1.0f},
                                      {0.5f, 0.8f, 1.0f}};

  GLuint VBO, VAO, colorBuffer;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &colorBuffer);

  glBindVertexArray(VAO);

  // Carrega os vértices no VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, skyVertices.size() * sizeof(glm::vec2),
               skyVertices.data(), GL_STATIC_DRAW);
  GLint posAttrib = glGetAttribLocation(m_program, "aPos");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  // Carrega as cores no colorBuffer
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, skyColors.size() * sizeof(glm::vec3),
               skyColors.data(), GL_STATIC_DRAW);
  GLint colorAttrib = glGetAttribLocation(m_program, "aColor");
  glEnableVertexAttribArray(colorAttrib);
  glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // Desenha o céu usando GL_TRIANGLE_STRIP
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Limpeza
  glDisableVertexAttribArray(posAttrib);
  glDisableVertexAttribArray(colorAttrib);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &colorBuffer);
  glDeleteVertexArrays(1, &VAO);
}

void Background::drawGround() const {
  std::vector<glm::vec2> groundVertices = {
      {-1.0f, 0.0f}, {1.0f, 0.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f}};
  std::vector<glm::vec3> groundColors = {{0.3f, 0.6f, 0.3f},
                                         {0.3f, 0.6f, 0.3f},
                                         {0.3f, 0.6f, 0.3f},
                                         {0.3f, 0.6f, 0.3f}};

  GLuint VBO, VAO, colorBuffer;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &colorBuffer);

  glBindVertexArray(VAO);

  // Carrega os vértices no VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, groundVertices.size() * sizeof(glm::vec2),
               groundVertices.data(), GL_STATIC_DRAW);
  GLint posAttrib = glGetAttribLocation(m_program, "aPos");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  // Carrega as cores no colorBuffer
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, groundColors.size() * sizeof(glm::vec3),
               groundColors.data(), GL_STATIC_DRAW);
  GLint colorAttrib = glGetAttribLocation(m_program, "aColor");
  glEnableVertexAttribArray(colorAttrib);
  glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // Desenha o chão usando GL_TRIANGLE_STRIP
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Limpeza
  glDisableVertexAttribArray(posAttrib);
  glDisableVertexAttribArray(colorAttrib);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &colorBuffer);
  glDeleteVertexArrays(1, &VAO);
}

void Background::drawMountain(float x, float y, float size) const {
  // Define os vértices da montanha (um triângulo)
  std::vector<glm::vec2> mountainVertices = {
      {x, y}, {x - size, y - size}, {x + size, y - size}};
  // Define as cores dos vértices da montanha (cinza)
  std::vector<glm::vec3> mountainColors = {
      {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}};

  GLuint VBO, VAO, colorBuffer;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &colorBuffer);

  glBindVertexArray(VAO);

  // Carrega os vértices no VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, mountainVertices.size() * sizeof(glm::vec2),
               mountainVertices.data(), GL_STATIC_DRAW);
  GLint posAttrib = glGetAttribLocation(m_program, "aPos");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  // Carrega as cores no colorBuffer
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, mountainColors.size() * sizeof(glm::vec3),
               mountainColors.data(), GL_STATIC_DRAW);
  GLint colorAttrib = glGetAttribLocation(m_program, "aColor");
  glEnableVertexAttribArray(colorAttrib);
  glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // Desenha a montanha
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Limpeza
  glDisableVertexAttribArray(posAttrib);
  glDisableVertexAttribArray(colorAttrib);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &colorBuffer);
  glDeleteVertexArrays(1, &VAO);
}

void Background::destroy() { glDeleteProgram(m_program); }
