#ifndef DISTRACTION_OBJECTS_HPP_
#define DISTRACTION_OBJECTS_HPP_

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"
#include <glm/vec2.hpp>
#include <list>
#include <random>
#include <string>
#include <vector>

class DistractionObjects {
public:
  struct DistractionObject {
    GLuint m_VAO{};
    GLuint m_VBO{};
    GLuint m_texture{};
    float m_angularVelocity{};
    bool checkClick(glm::vec2 const &clickPos) const;
    float m_rotation{0.0f};
    float m_scale{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{};
  };
  // Método para carregar textura
  GLuint loadTexture(std::string filepath);
  void create(GLuint program, int quantity);
  void paint();
  void destroy();
  void update(float deltaTime);
  bool checkClickOnDistraction(glm::vec2 clickPos);

private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  std::list<DistractionObject> m_distractions;
  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
};

#endif
