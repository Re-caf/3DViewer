#include "s21_Facade.hh"

namespace s21 {

ModelFacade::ModelFacade() : _parser() {
  // if (!loadModel("cube.obj")) {
  //     throw std::runtime_error("Failed to load the default model");
  // }
}

int ModelFacade::getCountVertexFacade() { return _parser.getCountVertex(); }

bool ModelFacade::loadModel(const std::string& filename) {
  _filename = filename;
  _parser.clearData();

  if (!_parser.ParseFile(filename)) {
    throw std::runtime_error("Failed to parse the file: " + filename);
  }

  // Проверка минимальных требований
  if (_parser.getCountVertex() < 3 || _parser.getCountFace() < 1) {
    throw std::runtime_error("Model must have at least 3 vertices and 1 face");
  }

  // Преобразование вершин
  std::vector<float> raw_vertices = _parser.getVertex();
  std::vector<Vertex> vertices;
  vertices.reserve(_parser.getCountVertex());

  for (size_t i = 0; i < raw_vertices.size(); i += 3) {
    vertices.emplace_back(
        Vertex{raw_vertices[i], raw_vertices[i + 1], raw_vertices[i + 2]});
  }

  // Получение индексов граней
  std::vector<long> indices = _parser.getFace();

  // Проверка на наличие данных
  if (vertices.empty() || indices.empty()) {
    throw std::runtime_error("Vertices or indices are empty after parsing");
  }

  // Создаем Mesh из vertices и indices
  Mesh mesh;
  mesh.vertices = std::move(vertices);
  mesh.indices = std::move(indices);

  // Инициализация модели через существующий конструктор
  _model = std::make_shared<WireframeModel>(mesh);

  // Нормализация масштаба (используем метод модели)
  maxVertex max = _parser.getMaxVertexs();
  float max_coord = std::max({max.x, max.y, max.z});
  if (max_coord > 0.0f) {
    _model->applyScaling(1.0f / max_coord);
  }

  return true;
}

void ModelFacade::applyTranslation(float x, float y, float z) {
  if (!_model) throw std::runtime_error("Model not loaded");

  TranslationCommand cmd(_model, x, y, z);
  cmd.execute();
  // _commandHistory.push(std::make_unique<TranslationCommand>(cmd));
  NotifyObservers();
}

void ModelFacade::applyRotation(float angleX, float angleY, float angleZ) {
  if (!_model) throw std::runtime_error("Model not loaded");

  RotationCommand cmd(_model, angleX, angleY, angleZ);
  cmd.execute();
  // _commandHistory.push(std::make_unique<RotationCommand>(cmd));
  NotifyObservers();
}

void ModelFacade::applyScaling(float scale) {
  if (!_model) throw std::runtime_error("Model not loaded");
  if (scale <= 0.0f) throw std::invalid_argument("Invalid scale value");

  ScalingCommand cmd(_model, scale);
  cmd.execute();
  // _commandHistory.push(std::make_unique<ScalingCommand>(cmd));
  NotifyObservers();
}

std::string ModelFacade::getFilename() const {
  return std::filesystem::path(_filename).filename().string();
}

// model_facade.cc
int ModelFacade::getCountEdgesFacade() {
  if (!_model) return 0;

  const auto& indices = _model->getMesh().indices;
  std::set<std::pair<long, long>> uniqueEdges;

  // Проходим по всем ребрам (парам вершин)
  for (size_t i = 0; i < indices.size(); i += 2) {
    if (i + 1 >= indices.size()) break;

    long v1 = indices[i];
    long v2 = indices[i + 1];

    // Создаем упорядоченную пару для уникальности
    long minV = std::min(v1, v2);
    long maxV = std::max(v1, v2);
    uniqueEdges.insert({minV, maxV});
  }

  return uniqueEdges.size();
}

}  // namespace s21
