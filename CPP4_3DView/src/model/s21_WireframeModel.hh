#ifndef S21_WIREFRAME_LIB
#define S21_WIREFRAME_LIB

#include <cmath>
#include <string>
#include <vector>

#include "../commons/s21_objects.hh"

namespace s21 {

/// @brief класс каркасной модели
class WireframeModel {
 public:
  /// @brief конструктор класса каркасной модели
  /// @param mesh меш, который описывает каркасную сетку
  WireframeModel(Mesh mesh) : mesh_(mesh) {}

  /// @brief метод смещения фигуры по осям
  /// @param x смещение по оси x
  /// @param y смещение по оси y
  /// @param z смещение по оси z
  void applyTranslation(float x, float y, float z);

  /// @brief метод поворота фигуры относительно осей
  /// @param angleX угол поворота относительно оси x
  /// @param angleY угол поворота относительно оси y
  /// @param angleZ угол поворота относительно оси z
  void applyRotation(float angleX, float angleY, float angleZ);

  /// @brief метод маштабирования фигуры на заданное значение
  /// @param scale
  void applyScaling(float scale);

  /// @brief метод, который возвращает фигуру
  /// @return
  const Mesh& getMesh() const { return mesh_; }

 private:
  /// @brief вектор мешей, который описывает каркасную сетку
  Mesh mesh_;
};

}  // namespace s21

#endif