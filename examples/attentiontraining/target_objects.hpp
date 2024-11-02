#ifndef TARGET_OBJECTS_HPP_
#define TARGET_OBJECTS_HPP_

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"
#include <list>
#include <random>

class TargetObjects{
public:
  void create(GLuint program, int quantity);
  void paint();
  void destroy();
  void update(float deltaTime);
  bool checkClickOnTarget(glm::vec2 clickPos);

  GLuint loadTexture(std::string filepath);

  struct TargetObject {
    GLuint m_VAO{};
    GLuint m_VBO{};
    GLuint m_texture{};
    float m_angularVelocity{};
    bool checkClick(glm::vec2 const &clickPos) const;
    float m_rotation{};
    float m_scale{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{};
  };

  std::list<TargetObject> m_targets;

private:
  GLuint m_program{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

};

#endif