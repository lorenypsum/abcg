#include "distraction_objects.hpp"
#include <SDL_image.h>
#include <iostream>

GLuint DistractionObjects::loadTexture(std::string filepath) {
  GLuint textureID;
  int width, height, channels;

  // Carregar a imagem usando stb_image
  SDL_Surface *surface = IMG_Load(filepath.c_str());
  if (!surface) {
    std::cerr << "Failed to load texture: " << filepath
              << " Error: " << IMG_GetError() << std::endl;
    return -1;
  }
  unsigned char *data = static_cast<unsigned char *>(surface->pixels);
  if (data) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_free(surface);
  } else {
    // Lidar com erro de carregamento
    std::cerr << "Failed to load texture: " << filepath << std::endl;
  }
  return textureID;
}

void DistractionObjects::create(GLuint program, int quantity) {
  m_program = program;
  auto const assetsPath{abcg::Application::getAssetsPath()};
  // Carregar a textura para o objeto de distração
  for (int i = 0; i < quantity; ++i) {
    DistractionObject obj;
    obj.m_texture =
        loadTexture(assetsPath + "/textures/basket_yellow.png"); // Exemplo de caminho

    // Inicializar VAO e VBO aqui (não mostrado)

    m_distractions.push_back(obj);
  }
}

void DistractionObjects::paint() {
  glUseProgram(m_program);
  for (auto &obj : m_distractions) {
    glBindTexture(GL_TEXTURE_2D, obj.m_texture); // Ativar a textura do objeto
    // Renderizar o objeto (não mostrado, deve incluir código para desenhar
    // usando VAO e VBO)
  }
  glUseProgram(0);
}

void DistractionObjects::destroy() {
  for (auto &obj : m_distractions) {
    glDeleteTextures(1, &obj.m_texture); // Liberar a textura
  }
}

bool DistractionObjects::checkClickOnDistraction(glm::vec2 clickPos) {
  for (auto &distraction : m_distractions) {
    if (distraction.checkClick(clickPos)) {
      return true; // Retorna verdadeiro se o clique foi em uma distração
    }
  }
  return false; // Retorna falso se o clique não foi em nenhuma distração
}

bool DistractionObjects::DistractionObject::checkClick(glm::vec2 const &clickPos) const {
  // Calcular os limites do objeto
  float halfSize = m_scale * 0.5f;
  glm::vec2 minBounds = m_translation - glm::vec2(halfSize);
  glm::vec2 maxBounds = m_translation + glm::vec2(halfSize);

  // Verificar se a posição do clique está dentro dos limites do objeto
  if (clickPos.x >= minBounds.x && clickPos.x <= maxBounds.x &&
      clickPos.y >= minBounds.y && clickPos.y <= maxBounds.y) {
    return true;
  }
  return false;
}