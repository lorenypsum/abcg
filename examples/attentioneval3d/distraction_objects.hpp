#ifndef DISTRACTION_OBJECTS_HPP_
#define DISTRACTION_OBJECTS_HPP_

#include "abcgOpenGL.hpp"
// #include "gamedata.hpp"
#include "model.hpp"
#include <glm/vec2.hpp>
#include <list>
#include <random>
#include <string>
#include <vector>

struct Vertexn {
  glm::vec3 position{};

  friend bool operator==(Vertexn const &, Vertexn const &) = default;
};

class DistractionObjects {
public:
  // star
  struct Star {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
  };
  std::array<Star, 100> m_stars;
  void randomizeStar(Star &star);

  // distraction objects
  struct DistractionObject {
    float m_angularVelocity{};
    bool checkClick(glm::vec3 const &clickPos) const;
    float m_rotation{0.0f};
    float m_scale{};
  };

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

  GLuint loadTexture(std::string filepath);

  void create();
  void paint();
  void paintUI();
  void destroy();
  void update(float deltaTime);
  bool checkClickOnDistraction(glm::vec3 const &clickPos,
                               glm::mat4 const &viewMatrix,
                               glm::mat4 const &projMatrix);
  void removeDistraction(std::list<DistractionObject>::iterator it);
  bool isEmpty() const;
  void loadObj(std::string_view path, bool standardize = true);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  glm::ivec2 m_viewportSize{};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  std::default_random_engine m_randomEngine;
  float m_FOV{30.0f};

private:
  Model m_model;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  // Shaders
  std::vector<char const *> m_shaderNames{
      "cubereflect", "cuberefract", "normalmapping", "texture", "blinnphong",
      "phong",       "gouraud",     "normal",        "depth"};
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  float m_angle{};
  std::list<DistractionObject> m_distractions;

  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  std::vector<Vertexn> m_vertices;
  std::vector<GLuint> m_indices;

  void createBuffers();
  void standardize();
  void loadModel(std::string_view path);
};

#endif
