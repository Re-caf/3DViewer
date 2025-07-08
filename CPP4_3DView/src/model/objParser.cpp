#include "objParser.hh"

namespace s21 {

int ObjParser::getCountVertex() { return m_countVertex; }

int ObjParser::getCountFace() { return m_countFace; }

std::vector<float> ObjParser::getVertex() { return m_verteces_; }

std::vector<long> ObjParser::getFace() { return m_faces; }

maxVertex ObjParser::getMaxVertexs() { return max_vertxs; }

bool ObjParser::ParseFile(const std::string &file_name) {
  std::ifstream file(file_name);
  std::string currLine;
  bool status = true;
  clearData();

  if (!file.is_open()) return false;

  while (std::getline(file, currLine) && status) {
    status = parsLine(currLine);
  }

  file.close();
  return status;
}

void ObjParser::maxVertexValue(float x, float y, float z) {
  max_vertxs.x = std::max(max_vertxs.x, std::fabs(x));
  max_vertxs.y = std::max(max_vertxs.y, std::fabs(y));  // !!!
  max_vertxs.z = std::max(max_vertxs.z, std::fabs(z));  // !!!
}

bool ObjParser::parsVertex(
    const std::string &data) {  // Парсит принятые координаты вершины
  std::istringstream dataStrim(data);
  float x = 0, y = 0, z = 0;
  if (dataStrim >> x >> y >> z) {
    m_verteces_.push_back(x);
    m_verteces_.push_back(y);
    m_verteces_.push_back(z);
    maxVertexValue(x, y, z);
    m_countVertex++;
  } else {
    clearData();
    return false;
  }
  return true;
}

bool ObjParser::parsLine(const std::string &line) {
  std::istringstream iss(line);
  std::string token;
  iss >> token;

  if (token == "v") {
    float x, y, z;
    if (iss >> x >> y >> z) return parsVertex(line.substr(2));  // !!!!
  } else if (token == "f") {
    std::string faceData = line.substr(1);
    return parsFace(faceData);
  }
  return true;
}

bool ObjParser::parsFace(const std::string &data) {
  std::istringstream dataStrim(data);
  std::string f_element;
  std::vector<long> answrFaceLine;

  while (dataStrim >> f_element) {
    if (!parseIndex(f_element, answrFaceLine)) {
      return false;  // Ошибка парсинга индекса
    }
  }

  // Проверяем что набрали достаточно индексов для грани
  if (answrFaceLine.size() < 3) {
    return false;  // Недостаточно вершин для полигона
  }

  // Добавляем грань только если все индексы валидны
  if (insertFaces(answrFaceLine)) {
    m_countFace++;  // Увеличиваем счетчик только после успешного добавления
    return true;
  }

  return false;
}

/************************************************** */

/*работает до оконания строки, обработка по 1 ЧИСЛУ*/
bool ObjParser::parseIndex(std::string &f_element,
                           std::vector<long> &answrFaceLine) {
  long index;

  if (!extract_F_element(f_element)) return false;

  if (!pars_F_element(f_element, index)) return false;

  if (!checkIndexRange(index)) return false;

  unsigned int correctedIndex =
      correctIndex(index);  // переводит отрицательный индекс

  return addFaceIndex(answrFaceLine, correctedIndex);
}

bool ObjParser::extract_F_element(
    std::string &f_element) {  // меняет 154/78/14 на 154
  size_t slash_pos = f_element.find('/');

  while (slash_pos != std::string::npos) {
    f_element.erase(slash_pos);
    slash_pos = f_element.find('/');
  }

  return true;
}

bool ObjParser::addFaceIndex(std::vector<long> &lineFaceIndx, unsigned index) {
  lineFaceIndx.push_back(index);
  return true;
}

bool ObjParser::pars_F_element(const std::string &f_element,
                               long &index) {  // функция для обработки строки
  try {
    index = std::stoi(f_element);
  } catch (const std::out_of_range &) {
    return false;
  }
  return true;
}

bool ObjParser::checkIndexRange(long index) {
  long abs_index = std::abs(index);
  return abs_index > 0 && abs_index <= m_countVertex;  // !!!!
}

unsigned int ObjParser::correctIndex(long index) {
  if (index < 0)
    return index + m_countVertex;  // !!!!
  else
    return index - 1;
}

bool ObjParser::insertFaces(const std::vector<long> &answrFaceLine) {
  if (answrFaceLine.size() < 3) return false;

  for (size_t i = 0; i < answrFaceLine.size(); ++i) {
    long from = answrFaceLine[i];
    long to = answrFaceLine[(i + 1) % answrFaceLine.size()];
    m_faces.push_back(from);
    m_faces.push_back(to);
  }

  return true;
}

};  // namespace s21
