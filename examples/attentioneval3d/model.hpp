#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "abcgOpenGL.hpp"

// Vértice com posição, normal e coordenadas de textura
struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};
  glm::vec4 tangent{};
  friend bool operator==(Vertex const &, Vertex const &) = default;
};

// Classe que representa um modelo 3D
class Model {
public:
  // void loadCubeTexture(std::string const &path);
  void loadNormalTexture(std::string_view path);

  // Funções de configuração do modelo
  void loadDiffuseTexture(std::string_view path);
  void loadObj(std::string_view path, bool standardize = true);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  void setSize(float size) { m_size = size; };
  void destroy();

  // Configura o número de triângulos
  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

  // Configurações de luz e material
  [[nodiscard]] glm::vec4 getKa() const { return m_Ka; }
  [[nodiscard]] glm::vec4 getKd() const { return m_Kd; }
  [[nodiscard]] glm::vec4 getKs() const { return m_Ks; }
  [[nodiscard]] float getShininess() const { return m_shininess; }
  [[nodiscard]] bool isUVMapped() const { return m_hasTexCoords; }
  // [[nodiscard]] GLuint getCubeTexture() const { return m_cubeTexture; }

private:
  // VAO, VBO e EBO
  GLuint m_normalTexture{};
  // GLuint m_cubeTexture{};
  GLuint m_diffuseTexture{};
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  // Shaders
  std::vector<char const *> m_shaderNames{
      "cubereflect", "cuberefract", "normalmapping", "texture", "blinnphong",
      "phong",       "gouraud",     "normal",        "depth"};
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Propriedade de luz e material
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};
  float m_size{1.0f};

  // Vértices e índices
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  // Flags
  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  // Funções de carregamento
  void computeNormals();
  void computeTangents();
  void createBuffers();
  void standardize();
};

#endif