#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <cmath>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#define MIN_LENCH 7

namespace s21 {

typedef struct {
  float x;
  float y;
  float z;
} maxVertex;

class ObjParser {
 public:
  bool ParseFile(const std::string& fileName);

  ObjParser() = default;
  ~ObjParser() = default;
  int getCountVertex();
  int getCountFace();
  std::vector<float> getVertex();
  std::vector<long> getFace();

  maxVertex getMaxVertexs();

  void clearData() {
    m_verteces_.clear();
    m_faces.clear();
    m_countFace = 0;
    m_countVertex = 0;  // Добавляем сброс счетчика вершин
    max_vertxs = {0.0, 0.0, 0.0};  // Сбрасываем максимальные значения
  }

 private:
  std::vector<float> m_verteces_;
  std::vector<long> m_faces;

  maxVertex max_vertxs = {0.0, 0.0, 0.0};

  int m_countVertex = 0;  // сколько у нас вершин с 3 координатами
  int m_countFace = 0;  // считает строки с f

  bool parsLine(const std::string& line);    // обработка строки
  bool parsVertex(const std::string& data);  // обработка вершин
  void maxVertexValue(float x, float y, float z);  // нужен для масштабирования

  bool parsFace(const std::string& data);
  bool parseIndex(
      std::string& f_element,
      std::vector<long>&
          answrFaceLine); /*работает до оконания строки, обработка по 1 ЧИСЛУ*/
  bool extract_F_element(std::string& f_element);  // меняет 154/78/14 на 154
  bool pars_F_element(const std::string& f_element, long& index);
  bool checkIndexRange(long index);
  unsigned int correctIndex(long index);

  bool addFaceIndex(std::vector<long>& lineFaceIndx, unsigned index);

  bool insertFaces(const std::vector<long>& answrFaceLine);
};
}  // namespace s21

#endif
