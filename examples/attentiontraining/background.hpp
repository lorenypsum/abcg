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
  void drawSky() const;
  void drawGround() const;
  void drawMountain(float x, float y, float size) const;
};

#endif
