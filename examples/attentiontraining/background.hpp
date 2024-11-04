#ifndef BACKGROUND_HPP_
#define BACKGROUND_HPP_

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"

class Background {
public:
  void create();
  void paint() const;
  void destroy();

private:
  GLuint m_program{};
  void drawSky(float heightOffset, glm::vec3 color) const;
  void drawGround(float heightOffset, glm::vec3 color) const;
  //void drawMountain(float x, float y, float size, float heightOffset) const;
  void drawMountain(float x, float y, float size, float heightOffset, glm::vec3 color) const;
};

#endif
