#ifndef S21_COMMANDS_LIB
#define S21_COMMANDS_LIB

#include <memory>

#include "../commons/s21_objects.hh"
#include "s21_WireframeModel.hh"
namespace s21 {
/// @brief интерфейс, реализуемый командами
class ICommand {
 private:
 public:
  virtual void execute() = 0;
  virtual void undo() = 0;
  virtual ~ICommand() noexcept = default;
};

/// @brief Базовый класс для команд преобразования
class TransformCommand : public ICommand {
 protected:
  std::shared_ptr<WireframeModel> model_;

  explicit TransformCommand(std::shared_ptr<WireframeModel> model)
      : model_(model) {
    if (!model) {
      throw std::invalid_argument("Model pointer cannot be null");
    }
  }
};

/// @brief команда на смещение модели по осям
class TranslationCommand : public TransformCommand {
 private:
  const float step_x, step_y, step_z;

 public:
  /// @brief Создает команду перемещения
  /// @param model Указатель на модель
  /// @param x Смещение по оси X
  /// @param y Смещение по оси Y
  /// @param z Смещение по оси Z
  TranslationCommand(std::shared_ptr<WireframeModel> model, float x, float y,
                     float z)
      : TransformCommand(std::move(model)), step_x(x), step_y(y), step_z(z) {}
  ~TranslationCommand() = default;

  void execute() override { model_->applyTranslation(step_x, step_y, step_z); }
  void undo() override { model_->applyTranslation(-step_x, -step_y, -step_z); }
};

/// @brief команда на поворот фигуры вокруг осей
class RotationCommand : public TransformCommand {
 private:
  float angleX, angleY, angleZ;

 public:
  /// @brief Создает команду поворота
  /// @param model Указатель на модель
  /// @param x Угол поворота вокруг оси X (в радианах)
  /// @param y Угол поворота вокруг оси Y (в радианах)
  /// @param z Угол поворота вокруг оси Z (в радианах)
  RotationCommand(std::shared_ptr<WireframeModel> model, float x, float y,
                  float z)
      : TransformCommand(std::move(model)), angleX(x), angleY(y), angleZ(z) {}
  ~RotationCommand() = default;

  void execute() override { model_->applyRotation(angleX, angleY, angleZ); }
  void undo() override { model_->applyRotation(-angleX, -angleY, -angleZ); }
};

/// @brief команда на маштабирование фигуры
class ScalingCommand : public TransformCommand {
 private:
  float scale;

 public:
  /// @brief Создает команду масштабирования
  /// @param model Указатель на модель
  /// @param scale Коэффициент масштабирования (должен быть положительным)
  /// @throws std::invalid_argument если scale <= 0
  ScalingCommand(std::shared_ptr<WireframeModel> model, float scale)
      : TransformCommand(std::move(model)), scale(scale) {
    if (scale <= 0) {
      throw std::invalid_argument("Scale factor must be positive");
    }
  }
  ~ScalingCommand() = default;

  void execute() override { model_->applyScaling(scale); }

  void undo() override { model_->applyScaling(1.0f / scale); }
};

}  // namespace s21

#endif