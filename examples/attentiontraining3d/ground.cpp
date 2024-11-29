#include "ground.hpp"
#include <SDL_image.h>
#include <iostream>

// Carrega a textura do objeto alvo
GLuint Ground::loadTexture(std::string filepath) {
  // Variável para armazenar o ID da textura
  GLuint textureID;

  // Carrega a imagem usando stb_image
  SDL_Surface *surface = IMG_Load(filepath.c_str());
  if (!surface) {
    std::cerr << "Failed to load texture: " << filepath
              << " Error: " << IMG_GetError() << std::endl;
    return -1;
  }

  // Define o formato da textura com base no canal alfa
  GLenum format = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;

  // Gera e configura a textura
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format,
               GL_UNSIGNED_BYTE, surface->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Libera a superfície após carregamento
  SDL_FreeSurface(surface);
  return textureID;
}

void Ground::create(GLuint program) {

  // Unit quad on the xz plane
  std::array<glm::vec3, 4> vertices{{{-0.5f, 0.0f, +0.5f},
                                     {-0.5f, 0.0f, -0.5f},
                                     {+0.5f, 0.0f, +0.5f},
                                     {+0.5f, 0.0f, -0.5f}}};

  std::array<glm::vec2, 4> texCoords{{{0.0f, 1.0f},
                                      {0.0f, 0.0f},
                                      {1.0f, 1.0f},
                                      {1.0f, 0.0f}}}; // Coordenadas de textura

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);

  auto const texCoordAttribute =
      abcg::glGetAttribLocation(program, "inTexCoord");
  abcg::glEnableVertexAttribArray(texCoordAttribute);
  abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              (void *)sizeof(vertices));

  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  // Get assets path
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Load Texture
  m_texture = loadTexture(
      (assetsPath + "/textures/ground.png").c_str()); // Carrega a textura

  // Get location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
  m_textureLoc = abcg::glGetUniformLocation(program, "textureSampler");
}

void Ground::paint() {


  abcg::glBindVertexArray(m_VAO);

  // Draw a grid of 2N+1 x 2N+1 tiles on the xz plane, centered around the
  // origin
  auto const N{5};
  for (auto const z : iter::range(-N, N + 1)) {
    for (auto const x : iter::range(-N, N + 1)) {
      // Set model matrix as a translation matrix
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(x, 0.0f, z));
      abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      // Bind the texture
      abcg::glActiveTexture(GL_TEXTURE0);
      abcg::glBindTexture(GL_TEXTURE_2D, m_texture);
      abcg::glUniform1i(m_textureLoc, 0); // Set the texture unit

      abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
  }

  abcg::glBindVertexArray(0);
}

void Ground::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
  abcg::glDeleteTextures(1, &m_texture);
}
