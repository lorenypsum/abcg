#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"

class Ground {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  GLuint loadTexture(std::string filepath);

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_texture{};
  GLint m_textureLoc{}; 
  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
};

#endif