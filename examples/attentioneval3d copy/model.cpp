#include "model.hpp"

#include <filesystem>
#include <unordered_map>

// Função de hash para o tipo Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    auto const h2{std::hash<glm::vec3>()(vertex.normal)};
    auto const h3{std::hash<glm::vec2>()(vertex.texCoord)};
    return abcg::hashCombine(h1, h2, h3);
  }
};

// Configura normal para o vetor
void Model::computeNormals() {
  // Limpa as normais anteriores
  for (auto &vertex : m_vertices) {
    vertex.normal = glm::vec3(0.0f);
  }

  // Computa face das normais
  for (auto const offset : iter::range(0UL, m_indices.size(), 3UL)) {
    // Obtém face dos vértices
    auto &a{m_vertices.at(m_indices.at(offset + 0))};
    auto &b{m_vertices.at(m_indices.at(offset + 1))};
    auto &c{m_vertices.at(m_indices.at(offset + 2))};

    // Computa normal
    auto const edge1{b.position - a.position};
    auto const edge2{c.position - b.position};
    auto const normal{glm::cross(edge1, edge2)};

    // Acumula nos vértices
    a.normal += normal;
    b.normal += normal;
    c.normal += normal;
  }

  // Normaliza as normais acumuladas
  for (auto &vertex : m_vertices) {
    vertex.normal = glm::normalize(vertex.normal);
  }

  m_hasNormals = true;
}

void Model::computeTangents() {
  // Reserva espaço para bitangents
  std::vector bitangents(m_vertices.size(), glm::vec3(0));

  // Computa face das tangentes e bitangentes
  for (auto const offset : iter::range(0UL, m_indices.size(), 3UL)) {
    // Obtém índices das faces
    auto const i1{m_indices.at(offset + 0)};
    auto const i2{m_indices.at(offset + 1)};
    auto const i3{m_indices.at(offset + 2)};

    // Obtém vértices das faces
    auto &v1{m_vertices.at(i1)};
    auto &v2{m_vertices.at(i2)};
    auto &v3{m_vertices.at(i3)};

    // Computa aresta e deltas
    auto const e1{v2.position - v1.position};
    auto const e2{v3.position - v1.position};
    auto const delta1{v2.texCoord - v1.texCoord};
    auto const delta2{v3.texCoord - v1.texCoord};

    // Computa matriz de tangentes
    glm::mat2 M;
    M[0][0] = delta2.t;
    M[0][1] = -delta1.t;
    M[1][0] = -delta2.s;
    M[1][1] = delta1.s;
    M *= (1.0f / (delta1.s * delta2.t - delta2.s * delta1.t));

    // Computa tangente
    auto const tangent{glm::vec4(M[0][0] * e1.x + M[0][1] * e2.x,
                                 M[0][0] * e1.y + M[0][1] * e2.y,
                                 M[0][0] * e1.z + M[0][1] * e2.z, 0.0f)};

    // Computa bitangente
    auto const bitangent{glm::vec3(M[1][0] * e1.x + M[1][1] * e2.x,
                                   M[1][0] * e1.y + M[1][1] * e2.y,
                                   M[1][0] * e1.z + M[1][1] * e2.z)};

    // Acumula nos vértices
    v1.tangent += tangent;
    v2.tangent += tangent;
    v3.tangent += tangent;

    // Acumula nos bitangents
    bitangents.at(i1) += bitangent;
    bitangents.at(i2) += bitangent;
    bitangents.at(i3) += bitangent;
  }

  // Normaliza tangentes e bitangentes
  for (auto &&[i, vertex] : iter::enumerate(m_vertices)) {
    auto const &n{vertex.normal};
    auto const &t{glm::vec3(vertex.tangent)};

    // Ortogonaliza t com relação ao n
    auto const tangent{t - n * glm::dot(n, t)};
    vertex.tangent = glm::vec4(glm::normalize(tangent), 0);

    // Computa 'handedness' da base re-ortogonalizada
    auto const b{glm::cross(n, t)};
    auto const handedness{glm::dot(b, bitangents.at(i))};
    vertex.tangent.w = (handedness < 0.0f) ? -1.0f : 1.0f;
  }
}

// Cria buffers
void Model::createBuffers() {
  // Deleta buffers anteriores
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);

  // Cria e configura VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Cria e configura EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// TODO: Usar textura de cubo para configuração de ambiente
// void Model::loadCubeTexture(std::string const &path) {
//   if (!std::filesystem::exists(path))
//     return;

//   abcg::glDeleteTextures(1, &m_cubeTexture);
//   m_cubeTexture = abcg::loadOpenGLCubemap(
//       {.paths = {path + "posx.jpg", path + "negx.jpg", path + "posy.jpg",
//                  path + "negy.jpg", path + "posz.jpg", path + "negz.jpg"}});
// }

// void Model::loadNormalTexture(std::string_view path) {
//   if (!std::filesystem::exists(path))
//     return;

//   abcg::glDeleteTextures(1, &m_normalTexture);
//   m_normalTexture = abcg::loadOpenGLTexture({.path = path});
// }

// Carrega textura difusa
void Model::loadDiffuseTexture(std::string_view path) {
  if (!std::filesystem::exists(path))
    return; // Verifica se o arquivo existe

  abcg::glDeleteTextures(1, &m_diffuseTexture); // Deleta textura anterior
  m_diffuseTexture = abcg::loadOpenGLTexture({.path = path}); // Carrega textura
}

// Carrega objeto
void Model::loadObj(std::string_view path, bool standardize) {
  auto const basePath{std::filesystem::path{path}.parent_path().string() +
                      "/"}; // Caminho base

  tinyobj::ObjReaderConfig readerConfig; // Configuração do leitor
  readerConfig.mtl_search_path =
      basePath; // Caminho para os arquivos de material (.mtl)

  tinyobj::ObjReader reader; // Leitor

  // Verifica se o arquivo foi carregado
  if (!reader.ParseFromFile(path.data(), readerConfig)) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  // Verifica se há avisos
  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  // Acesso aos atributos, formas e materiais
  auto const &attrib{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};
  auto const &materials{reader.GetMaterials()};

  // Limpa os vetores e índices
  m_vertices.clear();
  m_indices.clear();

  // Propriedades iniciais
  m_hasNormals = false;
  m_hasTexCoords = false;

  // Um map de chave:valor com chave=Vertex e valor=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop pelas formas (shapes)
  for (auto const &shape : shapes) {
    // Loop pelos índices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Acesso ao vértice
      auto const index{shape.mesh.indices.at(offset)};

      // Posição
      auto const startIndex{3 * index.vertex_index};
      glm::vec3 position{attrib.vertices.at(startIndex + 0),
                         attrib.vertices.at(startIndex + 1),
                         attrib.vertices.at(startIndex + 2)};

      // Normal
      glm::vec3 normal{};
      if (index.normal_index >= 0) {
        m_hasNormals = true;
        auto const normalStartIndex{3 * index.normal_index};
        normal = {attrib.normals.at(normalStartIndex + 0),
                  attrib.normals.at(normalStartIndex + 1),
                  attrib.normals.at(normalStartIndex + 2)};
      }

      // Coordenadas de Textura
      glm::vec2 texCoord{};
      if (index.texcoord_index >= 0) {
        m_hasTexCoords = true;
        auto const texCoordsStartIndex{2 * index.texcoord_index};
        texCoord = {attrib.texcoords.at(texCoordsStartIndex + 0),
                    attrib.texcoords.at(texCoordsStartIndex + 1)};
      }

      Vertex const vertex{
          .position = position, .normal = normal, .texCoord = texCoord};

      // Se o hash não contém o vértice
      if (!hash.contains(vertex)) {
        // Adiciona este índice (tamanho de m_vertices)
        hash[vertex] = m_vertices.size();
        // Adiciona este vértice
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }

  // Usa propriedades do primeiro material, se disponível
  if (!materials.empty()) {
    auto const &mat{materials.at(0)}; // Primeiro material
    m_Ka = {mat.ambient[0], mat.ambient[1], mat.ambient[2], 1};
    m_Kd = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1};
    m_Ks = {mat.specular[0], mat.specular[1], mat.specular[2], 1};
    m_shininess = mat.shininess;
    // if (!mat.normal_texname.empty()) {
    //   loadNormalTexture(basePath + mat.normal_texname);
    // } else if (!mat.bump_texname.empty()) {
    //   loadNormalTexture(basePath + mat.bump_texname);
    // }
    if (!mat.diffuse_texname.empty())
      loadDiffuseTexture(basePath + mat.diffuse_texname);

  } else {
    // Valores padrão
    m_Ka = {0.1f, 0.1f, 0.1f, 1.0f};
    m_Kd = {0.7f, 0.7f, 0.7f, 1.0f};
    m_Ks = {1.0f, 1.0f, 1.0f, 1.0f};
    m_shininess = 25.0f;
  }

  // Centraliza e normaliza o modelo se necessário
  if (standardize) {
    Model::standardize();
  }

  // Computa normais se não existirem
  if (!m_hasNormals) {
    computeNormals();
  }

  // Computa tangentes se houver coordenadas de textura
  if (m_hasTexCoords) {
    computeTangents();
  }

  // Cria buffers para o modelo
  createBuffers();
}

// Renderiza modelo
void Model::render(int numTriangles) const {
  abcg::glBindVertexArray(m_VAO);

  // Ativa e vincula a textura difusa
  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  // Define parâmetros de minificação e magnificação
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Define parâmetros de repetição de textura
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Calcula o número de índices a serem desenhados
  auto const numIndices{(numTriangles < 0) ? m_indices.size()
                                           : numTriangles * 3};

  // Desenha os elementos
  abcg::glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}

void Model::setupVAO(GLuint program) {
  // Libera o VAO anterior
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Cria e vincula um novo VAO
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);

  // Vincula EBO e VBO
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // Vincula atributos de vértice
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  if (positionAttribute >= 0) {
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex), nullptr);
  }

  auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
  if (normalAttribute >= 0) {
    abcg::glEnableVertexAttribArray(normalAttribute);
    auto const offset{offsetof(Vertex, normal)};
    abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  auto const texCoordAttribute{
      abcg::glGetAttribLocation(program, "inTexCoord")};
  if (texCoordAttribute >= 0) {
    abcg::glEnableVertexAttribArray(texCoordAttribute);
    auto const offset{offsetof(Vertex, texCoord)};
    abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  auto const tangentCoordAttribute{
      abcg::glGetAttribLocation(program, "inTangent")};
  if (tangentCoordAttribute >= 0) {
    abcg::glEnableVertexAttribArray(tangentCoordAttribute);
    auto const offset{offsetof(Vertex, tangent)};
    abcg::glVertexAttribPointer(tangentCoordAttribute, 4, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  // Fim do Binding
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);
}

void Model::standardize() {
  // Centraliza a origem e normaliza a largura do limite para [-1, 1]

  // Pega limites
  glm::vec3 max(std::numeric_limits<float>::lowest());
  glm::vec3 min(std::numeric_limits<float>::max());
  for (auto const &vertex : m_vertices) {
    max = glm::max(max, vertex.position);
    min = glm::min(min, vertex.position);
  }

  // Centraliza e escala
  auto const center{(min + max) / 2.0f};
  auto const scaling{2.0f / glm::length(max - min)};
  for (auto &vertex : m_vertices) {
    vertex.position = (vertex.position - center) * scaling;
  }
}

// Destrói buffers
void Model::destroy() {
  // Deleta texturas e buffers
  // abcg::glDeleteTextures(1, &m_cubeTexture);
  // abcg::glDeleteTextures(1, &m_normalTexture);
  abcg::glDeleteTextures(1, &m_diffuseTexture);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}