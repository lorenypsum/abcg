#ifndef BACKGROUND_HPP_
#define BACKGROUND_HPP_

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"

// Classe que representa o cenário do jogo
class Background {
public:
  void paint(GLuint program) const;
  void destroy();
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

private:
  GLuint m_program{};
  void drawSky(float heightOffset, glm::vec3 color) const;
  void drawGround(float heightOffset, glm::vec3 color) const;
  void drawMountain(float x, float y, float size, float heightOffset,
                    glm::vec3 color) const;
};

#endif
