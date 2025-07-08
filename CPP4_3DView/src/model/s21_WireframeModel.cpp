#include "s21_WireframeModel.hh"

namespace s21 {
void WireframeModel::applyTranslation(float x, float y, float z) {
  for (auto& vertex : mesh_.vertices) {
    vertex.position[0] += x;
    vertex.position[1] += y;
    vertex.position[2] += z;
  }
}

void WireframeModel::applyRotation(float angleX, float angleY, float angleZ) {
  float sinX = std::sin(angleX), cosX = std::cos(angleX);
  float sinY = std::sin(angleY), cosY = std::cos(angleY);
  float sinZ = std::sin(angleZ), cosZ = std::cos(angleZ);

  for (auto& vertex : mesh_.vertices) {
    float tempX = vertex.position[0];
    float tempY = vertex.position[1];
    float tempZ = vertex.position[2];

    // Поворот вокруг оси X
    float rotatedY = tempY * cosX - tempZ * sinX;
    float rotatedZ = tempY * sinX + tempZ * cosX;

    // Обновляем y и z для дальнейшего вращения
    tempY = rotatedY;
    tempZ = rotatedZ;

    // Поворот вокруг оси Y
    float rotatedX = tempX * cosY + tempZ * sinY;
    rotatedZ = -tempX * sinY + tempZ * cosY;

    // Обновляем x и z для дальнейшего вращения
    tempX = rotatedX;
    tempZ = rotatedZ;

    // Поворот вокруг оси Z
    rotatedX = tempX * cosZ - tempY * sinZ;
    rotatedY = tempX * sinZ + tempY * cosZ;

    // Обновляем координаты вершины
    vertex.position[0] = rotatedX;
    vertex.position[1] = rotatedY;
    vertex.position[2] = tempZ;  // Z уже обновлено на предыдущем этапе
  }
}

void WireframeModel::applyScaling(float scale) {
  for (auto& vertex : mesh_.vertices) {
    vertex.position[0] *= scale;
    vertex.position[1] *= scale;
    vertex.position[2] *= scale;
  }
}
}  // namespace s21