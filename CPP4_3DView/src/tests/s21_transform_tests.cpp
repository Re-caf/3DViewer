#include <gtest/gtest.h>

#include <cmath>

#include "../commons/s21_objects.hh"
#include "../model/s21_WireframeModel.hh"

// Вспомогательная функция для создания тестового меша
s21::Mesh createTestMesh() {
  s21::Mesh mesh;
  mesh.vertices = {{{0.0f, 0.0f, 0.0f}},
                   {{1.0f, 0.0f, 0.0f}},
                   {{0.0f, 1.0f, 0.0f}},
                   {{0.0f, 0.0f, 1.0f}}};
  mesh.indices = {0, 1, 2, 0, 2, 3};
  return mesh;
}

TEST(WireframeModelTest, ApplyTranslation) {
  s21::Mesh mesh = createTestMesh();
  s21::WireframeModel model(mesh);

  model.applyTranslation(1.0f, 2.0f, 3.0f);

  EXPECT_FLOAT_EQ(model.getMesh().vertices[0].position[0], 1.0f);
  EXPECT_FLOAT_EQ(model.getMesh().vertices[0].position[1], 2.0f);
  EXPECT_FLOAT_EQ(model.getMesh().vertices[0].position[2], 3.0f);

  EXPECT_FLOAT_EQ(model.getMesh().vertices[1].position[0], 2.0f);
  EXPECT_FLOAT_EQ(model.getMesh().vertices[1].position[1], 2.0f);
  EXPECT_FLOAT_EQ(model.getMesh().vertices[1].position[2], 3.0f);
}

TEST(WireframeModelTest, ApplyScaling) {
  s21::Mesh mesh = createTestMesh();
  s21::WireframeModel model(mesh);

  model.applyScaling(2.0f);

  EXPECT_FLOAT_EQ(model.getMesh().vertices[0].position[0], 0.0f);
  EXPECT_FLOAT_EQ(model.getMesh().vertices[0].position[1], 0.0f);
  EXPECT_FLOAT_EQ(model.getMesh().vertices[0].position[2], 0.0f);

  EXPECT_FLOAT_EQ(model.getMesh().vertices[1].position[0], 2.0f);
  EXPECT_FLOAT_EQ(model.getMesh().vertices[1].position[1], 0.0f);
  EXPECT_FLOAT_EQ(model.getMesh().vertices[1].position[2], 0.0f);
}

TEST(WireframeModelTest, ApplyRotation) {
  s21::Mesh mesh = createTestMesh();
  s21::WireframeModel model(mesh);

  model.applyRotation(
      0.0f, 0.0f,
      static_cast<float>(M_PI) / 2);  // Вращение на 90 градусов вокруг Z

  EXPECT_NEAR(model.getMesh().vertices[1].position[0], 0.0f, 1e-5);
  EXPECT_NEAR(model.getMesh().vertices[1].position[1], 1.0f, 1e-5);
  EXPECT_NEAR(model.getMesh().vertices[1].position[2], 0.0f, 1e-5);

  EXPECT_NEAR(model.getMesh().vertices[2].position[0], -1.0f, 1e-5);
  EXPECT_NEAR(model.getMesh().vertices[2].position[1], 0.0f, 1e-5);
  EXPECT_NEAR(model.getMesh().vertices[2].position[2], 0.0f, 1e-5);
}