#ifndef GAME_OBJECTS_HPP_
#define GAME_OBJECTS_HPP_

#include "abcgOpenGL.hpp"
// #include "gamedata.hpp"
#include "model.hpp"
#include <glm/vec2.hpp>
#include <list>
#include <random>
#include <string>
#include <vector>

struct PositionVertex {
  glm::vec3 position{};

  friend bool operator==(PositionVertex const &,
                         PositionVertex const &) = default;
};

class GameEntities {
public:
  // star
  struct SpatialObject {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
  };

  std::vector<SpatialObject> m_distactionObjects =
      std::vector<SpatialObject>(100);
  std::vector<SpatialObject> m_targetObjects = std::vector<SpatialObject>(100);
  void randomizeStar(SpatialObject &star, float minX, float maxX, float minY,
                     float maxY, float minZ, float maxZ);

  // distraction objects
  struct GameObject {
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
  void setupSpatialObjects(std::vector<SpatialObject> &m_distactionObjects,
                           float minX, float maxX, float minY, float maxY,
                           float minZ, float maxZ);
  void createDistraction(const std::string &assetsPath);
  void createObject(Model &m_model, const std::string &assetsPath,
                    const std::string &objPath,
                    const std::string &texturePaths);
  void renderObject(Model &m_model, const GLint KaLoc, const GLint KdLoc,
                    const GLint KsLoc, const GLint shininessLoc,
                    const GLint modelMatrixLoc,
                    std::vector<SpatialObject> &m_distactionObjects);
  void paint();

  void setModelVariables(const GLint KaLoc, const GLint KdLoc,
                         const GLint KsLoc, const GLint shininessLoc,
                         const GLint modelMatrixLoc);
  void paintUI();
  void destroy();
  void update(float deltaTime);
  void updateSpatialObjects(std::vector<SpatialObject> &m_st, float deltaTime,
                            float incZ, float posZ, float minX, float maxX,
                            float minY, float maxY, float minZ, float maxZ);
  void updateObjects(float deltaTime);
  bool isEmpty() const;
  bool checkClickOnObject(glm::vec3 const &clickPos,
                          glm::mat4 const &viewMatrix,
                          glm::mat4 const &projMatrix,
                          std::vector<SpatialObject> &m_distactionObjects);
  void removeObject(std::list<GameObject>::iterator it);
  void loadObj(std::string_view path, bool standardize = true);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  glm::ivec2 m_viewportSize{};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  std::default_random_engine m_randomEngine;
  float m_FOV{30.0f};

private:
  Model m_distraction;
  Model m_target;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  float m_angle{};
  std::list<GameObject> m_distractions;
  std::list<GameObject> m_targets;

  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  std::vector<PositionVertex> m_vertices;
  std::vector<GLuint> m_indices;

  void createBuffers();
  void standardize();
  void loadModel(std::string_view path);
};

#endif
