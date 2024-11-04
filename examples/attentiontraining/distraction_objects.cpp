#include "distraction_objects.hpp"
#include <SDL_image.h>
#include <iostream>

// Carrega a textura do objeto de distração
GLuint DistractionObjects::loadTexture(std::string filepath) {
  //
  GLuint textureID;

  // Carregar a imagem usando stb_image
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
  SDL_FreeSurface(surface); // Libera a superfície após carregamento
  return textureID;
}

// Cria objetos de distração
void DistractionObjects::create(GLuint program, int quantity) {
  // Limpa a lista de objetos de distração
  destroy();
  // Define o programa de shader
  m_program = program;
  // Define o número de objetos que deseja criar
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Carrega a textura para o objeto de distração
  for (int i = 0; i < quantity; ++i) {
    DistractionObject obj;
    obj.m_texture = loadTexture((assetsPath + "/textures/brownpigeon.png")
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
    obj.m_translation =
        glm::vec2(m_randomDist(m_randomEngine), m_randomDist(m_randomEngine));
    obj.m_scale = 0.5f;

    // Adiciona o objeto à lista de distrações
    m_distractions.push_back(obj);
  }
}

// Renderiza os objetos de distração
void DistractionObjects::paint() {
  // Ativa o programa de shader
  glUseProgram(m_program);
  // Renderiza cada objeto de distração
  for (auto &obj : m_distractions) {
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
// Libera recursos dos objetos de distração
void DistractionObjects::destroy() {
  for (auto &obj : m_distractions) {
    glDeleteTextures(1, &obj.m_texture); // Liberar a textura
  }
}
// Atualiza a posição dos objetos de distração
void DistractionObjects::update(float deltaTime) {
  for (auto it = m_distractions.begin(); it != m_distractions.end();) {
    it->m_translation += it->m_velocity * deltaTime * 100.0f;
    if (it->m_translation.x > 1.0f || it->m_translation.x < -1.0f)
      it->m_velocity.x *= -1.0f;
    if (it->m_translation.y > 1.0f || it->m_translation.y < -1.0f)
      it->m_velocity.y *= -1.0f;

    ++it; // Incrementa o iterador
  }

  // Verifica se a lista está vazia e recria objetos
  if (m_distractions.empty()) {
    // Define o número de objetos a ser criado
    create(m_program, 6);
  }
}

// Verifica se o clique foi em um objeto de distração
bool DistractionObjects::checkClickOnDistraction(glm::vec2 clickPos) {
  for (auto it = m_distractions.begin(); it != m_distractions.end(); ++it) {
    if (it->checkClick(clickPos)) {
      // Remove o objeto clicado
      removeDistraction(it);
      // Verifica se a lista está vazia e recria objetos
      if (m_distractions.empty()) {
        // Define o número de objetos novaemente
        create(m_program, 6);
      }
      // Retorna verdadeiro se o clique foi em uma distração
      return true;
    }
  }
  // Retorna falso se o clique não foi em nenhuma distração
  return false;
}

// Remove um objeto de distração da lista
void DistractionObjects::removeDistraction(
    std::list<DistractionObject>::iterator it) {
  // Libera a textura
  glDeleteTextures(1, &it->m_texture);
  // Remove o objeto da lista
  m_distractions.erase(it);
}

// Verifica se a lista de distrações está vazia
bool DistractionObjects::DistractionObject::checkClick(
    glm::vec2 const &clickPos) const {
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