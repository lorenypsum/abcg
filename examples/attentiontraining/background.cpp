#include "background.hpp"
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

// Converte um valor hexadecimal em um vetor de 3 componentes
glm::vec3 hexToVec3(uint32_t hexValue) {
  float red = ((hexValue >> 16) & 0xFF) / 255.0f;
  float green = ((hexValue >> 8) & 0xFF) / 255.0f;
  float blue = (hexValue & 0xFF) / 255.0f;

  return glm::vec3(red, green, blue);
}

// Cores usadas no cenário
glm::vec3 limegreen = hexToVec3(0xA4B509);
glm::vec3 darkpastelgreen = hexToVec3(0x6F8E74);
glm::vec3 lightpastelgreen = hexToVec3(0xC4D5C5);
glm::vec3 brown = hexToVec3(0x444003);
glm::vec3 aquablue = hexToVec3(0x03ABB3);
glm::vec3 lightbrown = hexToVec3(0x8A7D62);
glm::vec3 lightblue = hexToVec3(0xB4DEF5);

// Inicializa o cenário
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

// Desenha o cenário
void Background::paint() const {
  glUseProgram(m_program);

  // Desenha o céu
  drawSky(0.0f, lightblue);

  // Desenha montanhas de fundo no cenário
  drawMountain(-0.8f, -0.2f, 0.4f, 0.1f, lightpastelgreen);
  drawMountain(-0.3f, -0.2f, 0.6f, 0.3f, lightpastelgreen);
  drawMountain(0.3f, -0.2f, 0.6f, 0.3f, lightpastelgreen);
  drawMountain(0.7f, -0.2f, 0.5f, 0.2f, lightpastelgreen);

  // Desenha montanhas de frente no cenário
  drawMountain(-0.8f, -0.2f, 0.4f, 0.0f, darkpastelgreen);
  drawMountain(0.0f, -0.2f, 0.6f, 0.2f, darkpastelgreen);
  drawMountain(0.7f, -0.2f, 0.5f, 0.0f, darkpastelgreen);

  // Desenha o chão
  drawGround(0.0f, limegreen);

  // Desenha o rio
  drawGround(0.3f, aquablue);

  // Desenha o chão
  drawGround(0.9f, limegreen);

  // Desativa o programa do shader após desenhar
  glUseProgram(0);
}

// Desenha o céu
void Background::drawSky(float heightOffset, glm::vec3 color) const {
  // Define os vértices do céu
  std::vector<glm::vec2> skyVertices = {{-1.0f, 1.0f},
                                        {1.0f, 1.0f},
                                        {-1.0f, 0.0f + (heightOffset)},
                                        {1.0f, 0.0f + (heightOffset)}};
  std::vector<glm::vec3> skyColors = {{color}, {color}, {color}, {color}};

  // Cria e configura os buffers de vértices e cores
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

// Desenha o chão
void Background::drawGround(float heightOffset, glm::vec3 color) const {
  // Define os vértices do chão
  std::vector<glm::vec2> groundVertices = {{-1.0f, 0.0f},
                                           {1.0f, 0.0f},
                                           {-1.0f, -1.0f + (heightOffset)},
                                           {1.0f, -1.0f + (heightOffset)}};
  std::vector<glm::vec3> groundColors = {{color}, {color}, {color}, {color}};

  // Cria e configura os buffers de vértices e cores
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

// Desenha uma montanha
void Background::drawMountain(float x, float y, float size, float heightOffset,
                              glm::vec3 color) const {
  // Alinha a montanha ao início do chão (y = 0.0f)
  y = 0.4f + heightOffset;

  // Define os vértices da montanha (um triângulo)
  std::vector<glm::vec2> mountainVertices = {
      {x, y}, {x - size, y - size}, {x + size, y - size}};

  // Define as cores dos vértices da montanha (cinza)
  std::vector<glm::vec3> mountainColors = {color, color, color};

  // Cria e configura os buffers de vértices e cores
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

// Destroi o cenário
void Background::destroy() { glDeleteProgram(m_program); }
