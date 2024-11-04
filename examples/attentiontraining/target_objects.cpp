#include "target_objects.hpp"
#include <SDL_image.h>
#include <iostream>

// Carrega a textura do objeto alvo
GLuint TargetObjects::loadTexture(std::string filepath) {
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

// Cria objetos de alvo
void TargetObjects::create(GLuint program, int quantity) {
  // Limpa a lista de objetos de distração
  destroy();

  // Define o programa de shader
  m_program = program;

  // Define o número de objetos que deseja criar
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Carrega a textura para o objeto de distração
  for (int i = 0; i < quantity; ++i) {
    TargetObject obj;
    obj.m_texture = loadTexture((assetsPath + "/textures/whitepigeon.png")
                                    .c_str()); // Carrega a textura

    // Adiciona uma velocidade aleatória para movimento
    obj.m_velocity = glm::vec2(m_randomDist(m_randomEngine) * 0.005f,
                               m_randomDist(m_randomEngine) * 0.005f);

    // Inicializa o VAO e o VBO para o objeto
    glGenVertexArrays(1, &obj.m_VAO);
    glBindVertexArray(obj.m_VAO);

    // Define os vértices do objeto (um quadrado simples)
    GLfloat vertices[] = {
        //   Posição     | Coord. Textura
        -0.1f, -0.1f, 0.0f, 1.0f, // Inferior esquerdo (Y invertido)
        0.1f,  -0.1f, 1.0f, 1.0f, // Inferior direito (Y invertido)
        -0.1f, 0.1f,  0.0f, 0.0f, // Superior esquerdo (Y invertido)
        0.1f,  0.1f,  1.0f, 0.0f  // Superior direito (Y invertido)
    };

    // Cria e configura o VBO
    glGenBuffers(1, &obj.m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, obj.m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configura os atributos de posição
    GLint posAttrib = glGetAttribLocation(m_program, "inPosition");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          nullptr);

    // Configura os atributos de coordenada de textura
    GLint texCoordAttrib = glGetAttribLocation(m_program, "inTexCoord");
    glEnableVertexAttribArray(texCoordAttrib);
    glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float),
                          reinterpret_cast<void *>(2 * sizeof(float)));
    // Desabilita o VBO e o VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Inicializa as propriedades do objeto (posição e escala)
    float areaX = 0.7f; // Largura da área de aparição em X
    float areaY = 0.7f; // Altura da área de aparição em Y

    // Inicializa as propriedades do objeto (posição e escala)
    obj.m_translation =
        glm::vec2(m_randomDist(m_randomEngine) * areaX - areaX / 2.0f,
                  m_randomDist(m_randomEngine) * areaY - areaY / 2.0f);
    obj.m_scale = 0.5f;

    // Adiciona o objeto à lista
    m_targets.push_back(obj);
  }
}
// Renderiza os objetos de alvo
void TargetObjects::paint() {
  // Ativa o programa de shader
  glUseProgram(m_program);
  // Renderiza cada objeto de alvo
  for (auto &obj : m_targets) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, obj.m_texture);

    // Enviar translação, rotação e escala para o shader
    GLint translationLoc = glGetUniformLocation(m_program, "translation");
    GLint scaleLoc = glGetUniformLocation(m_program, "scale");
    GLint rotationLoc = glGetUniformLocation(m_program, "rotation");
    glUniform2fv(translationLoc, 1, &obj.m_translation[2]);
    glUniform1f(scaleLoc, obj.m_scale);
    glUniform1f(rotationLoc, obj.m_rotation);

    // Renderizar o objeto
    glBindVertexArray(obj.m_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
  }
  // Desativa o programa de shader
  glUseProgram(0);
}

// Libera recursos dos objetos de alvo
void TargetObjects::destroy() {
  for (auto &obj : m_targets) {
    glDeleteTextures(1, &obj.m_texture); // Liberar a textura
  }
}

// Atualiza a posição dos objetos de alvo
void TargetObjects::update(float deltaTime) {
  // Atualiza a posição de cada objeto
  for (auto it = m_targets.begin(); it != m_targets.end();) {
    it->m_translation += it->m_velocity * deltaTime * 100.0f;
    if (it->m_translation.x > 1.0f || it->m_translation.x < -1.0f)
      it->m_velocity.x *= -1.0f;
    if (it->m_translation.y > 1.0f || it->m_translation.y < -1.0f)
      it->m_velocity.y *= -1.0f;

    ++it; // Incrementa o iterador
  }
  // Verifica se a lista está vazia e recria objetos
  if (m_targets.empty()) {
    // Define o número de objetos a ser criado
    create(m_program, 1);
  }
}

// Verifica se o clique foi em um objeto de alvo
bool TargetObjects::checkClickOnTarget(glm::vec2 clickPos) {
  // Verifica cada objeto de alvo
  for (auto it = m_targets.begin(); it != m_targets.end(); ++it) {
    if (it->checkClick(clickPos)) {
      removeTarget(it); // Remove o objeto clicado
      // Verifica se a lista está vazia e recria objetos
      if (m_targets.empty()) {
        // Define o número de objetos novaemente
        create(m_program, 1);
      }
      // Retorna verdadeiro se o clique foi em uma distração
      return true;
    }
  }
  // Retorna falso se o clique não foi em nenhuma distração
  return false;
}

// Remove um objeto de alvo da lista
void TargetObjects::removeTarget(std::list<TargetObject>::iterator it) {
  glDeleteTextures(1, &it->m_texture); // Liberar a textura
  m_targets.erase(it);                 // Remove o objeto da lista
}

// Verifica se a lista de objetos de alvo está vazia
bool TargetObjects::TargetObject::checkClick(glm::vec2 const &clickPos) const {
  // Calcula os limites do objeto
  float halfSize = m_scale * 0.1f;
  glm::vec2 minBounds = m_translation - glm::vec2(halfSize);
  glm::vec2 maxBounds = m_translation + glm::vec2(halfSize);

  // Verifica se a posição do clique está dentro dos limites do objeto
  if (clickPos.x >= minBounds.x && clickPos.x <= maxBounds.x &&
      clickPos.y >= minBounds.y && clickPos.y <= maxBounds.y) {
    // Retorna verdadeiro se o clique foi em uma distração
    return true;
  }
  // Retorna falso se o clique não foi em nenhuma distração
  return false;
}
