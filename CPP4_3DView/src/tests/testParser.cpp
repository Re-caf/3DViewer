#include <gtest/gtest.h>

#include <chrono>
#include <fstream>

#include "objParser.hh"

class ObjParserTest : public ::testing::Test {
 protected:
  s21::ObjParser parser;

  void SetUp() override {
    // Создаём простой OBJ-файл для тестирования
    std::ofstream testFile("test.obj");
    testFile << "# Simple cube\n"
             << "v 0.0 0.0 0.0\n"
             << "v 1.0 0.0 0.0\n"
             << "v 1.0 1.0 0.0\n"
             << "v 0.0 1.0 0.0\n"
             << "v 0.0 0.0 1.0\n"
             << "v 1.0 0.0 1.0\n"
             << "v 1.0 1.0 1.0\n"
             << "v 0.0 1.0 1.0\n"
             << "f 1 2 3 4\n"   // 1 грань (4 вершины)
             << "f 5 6 7 8\n"   // 2 грань
             << "f 1 2 6 5\n"   // 3 грань
             << "f 2 3 7 6\n"   // 4 грань
             << "f 3 4 8 7\n"   // 5 грань
             << "f 4 1 5 8\n";  // 6 грань
    testFile.close();
  }

  void TearDown() override {
    // Удаляем тестовый файл
    remove("test.obj");
  }
};

TEST_F(ObjParserTest, ParseValidFileWithFaceCount) {
  bool result = parser.ParseFile("test.obj");

  ASSERT_TRUE(result);
  ASSERT_EQ(parser.getCountVertex(), 8);
  ASSERT_EQ(parser.getCountFace(), 6);  // Ожидаем 6 граней для куба
}

TEST_F(ObjParserTest, EmptyFileHasZeroFaces) {
  // Создаём пустой файл
  std::ofstream emptyFile("empty.obj");
  emptyFile.close();

  bool result = parser.ParseFile("empty.obj");

  ASSERT_TRUE(result);
  ASSERT_EQ(parser.getCountFace(), 0);

  remove("empty.obj");
}

TEST_F(ObjParserTest, FileWithOnlyVerticesHasZeroFaces) {
  // Создаём файл только с вершинами
  std::ofstream vertFile("vertices.obj");
  vertFile << "v 0.0 0.0 0.0\n"
           << "v 1.0 0.0 0.0\n"
           << "v 1.0 1.0 0.0\n";
  vertFile.close();

  bool result = parser.ParseFile("vertices.obj");

  ASSERT_TRUE(result);
  ASSERT_EQ(parser.getCountVertex(), 3);
  ASSERT_EQ(parser.getCountFace(), 0);  // Нет граней

  remove("vertices.obj");
}

TEST_F(ObjParserTest, DifferentFaceFormats) {
  // Создаём файл с разными форматами граней
  std::ofstream complexFile("complex.obj");
  complexFile << "v 0 0 0\nv 1 0 0\nv 1 1 0\nv 0 1 0\n"
              << "f 1 2 3\n"           // треугольник (1 грань)
              << "f 1 2 3 4\n"         // квад (2 грань)
              << "f 1//1 2//1 3//1\n"  // с нормалями (3 грань)
              << "f 1/1 2/1 3/1 4/1\n";  // с текстурами (4 грань)
  complexFile.close();

  bool result = parser.ParseFile("complex.obj");

  ASSERT_TRUE(result);
  ASSERT_EQ(parser.getCountFace(), 4);  // Все 4 должны быть посчитаны

  remove("complex.obj");
}

TEST_F(ObjParserTest, ClearDataResetsFaceCount) {
  // Сначала парсим файл с гранями
  parser.ParseFile("test.obj");
  ASSERT_EQ(parser.getCountFace(), 6);  // Проверяем, что грани загрузились

  // Очищаем данные
  parser.clearData();

  // Проверяем, что все счётчики сбросились
  ASSERT_EQ(parser.getCountVertex(), 0);
  ASSERT_EQ(parser.getCountFace(),
            0);  // Теперь проверяем сброс счётчика граней
}

TEST_F(ObjParserTest, MaxVertexCalculation) {
  parser.ParseFile("test.obj");
  s21::maxVertex max = parser.getMaxVertexs();

  ASSERT_FLOAT_EQ(max.x, 1.0f);
  ASSERT_FLOAT_EQ(max.y, 1.0f);
  ASSERT_FLOAT_EQ(max.z, 1.0f);

  parser.clearData();
  max = parser.getMaxVertexs();
  ASSERT_FLOAT_EQ(max.x, 0.0f);
  ASSERT_FLOAT_EQ(max.y, 0.0f);
  ASSERT_FLOAT_EQ(max.z, 0.0f);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}