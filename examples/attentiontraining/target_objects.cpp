#include "target_objects.hpp"
#include <SDL_image.h>
#include <iostream>

GLuint TargetObjects::loadTexture(std::string filepath) {
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

void TargetObjects::create(GLuint program, int quantity) {
  m_program = program;

  auto const assetsPath{abcg::Application::getAssetsPath()};
  // Carregar a textura para o objeto de distração
  for (int i = 0; i < quantity; ++i) {
    TargetObject obj;
    obj.m_texture =
        loadTexture(assetsPath + "/textures/banana.png"); // Exemplo de caminho

    // Inicializar VAO e VBO aqui (não mostrado)

    m_targets.push_back(obj);
  }
}

void TargetObjects::paint() {
  glUseProgram(m_program);
  for (auto &obj : m_targets) {
    glBindTexture(GL_TEXTURE_2D, obj.m_texture); // Ativar a textura do objeto
    // Renderizar o objeto (não mostrado, deve incluir código para desenhar
    // usando VAO e VBO)
  }
  glUseProgram(0);
}

void TargetObjects::destroy() {
  for (auto &obj : m_targets) {
    glDeleteTextures(1, &obj.m_texture); // Liberar a textura
  }
}

bool TargetObjects::checkClickOnTarget(glm::vec2 clickPos) {
  for (auto &target : m_targets) {
    if (target.checkClick(clickPos)) {
      return true; // Retorna verdadeiro se o clique foi em uma distração
    }
  }
  return false; // Retorna falso se o clique não foi em nenhuma distração
}

bool TargetObjects::TargetObject::checkClick(glm::vec2 const &clickPos) const {
  // Calculate the distance from the click position to the target's center
  glm::vec2 diff = clickPos - m_translation;
  float distance = glm::length(diff);

  // Check if the click is within the target's radius
  return distance <= m_scale;
}
