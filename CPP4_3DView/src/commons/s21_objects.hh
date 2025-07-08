#ifndef S21_OBJECTS_LIB
#define S21_OBJECTS_LIB

#include <string>
#include <vector>

namespace s21 {
/// @brief Вершина, а именно ее координаты(x, y, z)
struct Vertex {
  float position[3];  ///< x, y, z координаты
};

/// @brief Меш (содержит массивы вершин и индексов)
struct Mesh {
  std::vector<Vertex> vertices;  ///< вершины
  std::vector<long> indices;  ///< индексы вершин, образующих грани

  // Метаданные модели
  std::string filename;
  size_t vertexCount() const { return vertices.size(); }
  size_t faceCount() const { return indices.size(); }
};

}  // namespace s21

#endif