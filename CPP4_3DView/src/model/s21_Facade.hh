#ifndef S21_FACADE_LIB
#define S21_FACADE_LIB

#include <filesystem>
#include <iostream>
#include <list>
#include <memory>

#include "../commons/s21_objects.hh"
#include "../commons/s21_observer.hh"
#include "objParser.hh"
#include "s21_WireframeModel.hh"
#include "s21_commands.hh"
namespace s21 {

/// @brief интерфейс взаимодействия с моделей
class ModelFacade : public IObservable {
 private:
  /// @brief модель
  std::shared_ptr<WireframeModel> _model = nullptr;
  /// @brief парсер
  ObjParser _parser;

  std::string _filename;

 public:
  /// @brief конструктор
  ModelFacade();
  /// @brief деструктор
  ~ModelFacade() = default;

  /// @brief метод загрузки новой каркасной модели из файла
  /// @param filename путь к файлу
  /// @return true если загрузка прошла успешно, false в противном случае
  bool loadModel(const std::string& filename);

  /// @brief метод применения трансформаций к модели
  /// @param x смещение по оси x
  /// @param y смещение по оси y
  /// @param z смещение по оси z
  void applyTranslation(float x, float y, float z);

  /// @brief метод применения поворотов к модели
  /// @param angleX угол поворота по оси x
  /// @param angleY угол поворота по оси y
  /// @param angleZ угол поворота по оси z
  void applyRotation(float angleX, float angleY, float angleZ);

  /// @brief метод применения масштабирования к модели
  /// @param scale коэффициент масштабирования
  void applyScaling(float scale);

  /// @brief получить текущую каркасную модель в виде данных для отрисовки
  /// @return константная ссылка на текущую модель
  const Mesh& getMesh() const {
    if (!_model) {
      throw std::runtime_error(
          "Attempted to access mesh before model was loaded.");
    }
    return _model->getMesh();
  }

  bool hasModel() const { return static_cast<bool>(_model); }

  int getCountVertexFacade();
  int getCountEdgesFacade();

  std::string getFilename() const;
};
}  // namespace s21

#endif