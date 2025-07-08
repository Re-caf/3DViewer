#ifndef OPENGL_IMPL_H
#define OPENGL_IMPL_H

// Qt библиотеки для UI и OpenGL
#include <QColorDialog>
#include <QFileDialog>
#include <QImage>
#include <QLCDNumber>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLWidget>
#include <QSlider>
#include <QTimer>
#include <cmath>
#include <iostream>
#include <limits>

// Ваши пользовательские классы
#include "../commons/s21_objects.hh"
#include "gifimage/qgifimage.h"
#include "s21_Facade.hh"
#include "s21_ProgramSettings.hh"

// Класс OpenGLImpl отвечает за визуализацию 3D модели и взаимодействие с
// пользователем
class OpenGLImpl : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit OpenGLImpl(QWidget* parent = nullptr);  // Конструктор
  ~OpenGLImpl() override;                          // Деструктор

  void screenWidget(int index);  // Скриншот или отрисовка по индексу
  void setModelFacade(
      s21::ModelFacade* facade);  // Установка фасада модели (MVC)

  // Применить настройки из структуры Settings
  void applySettings(const s21::Settings& settings);
  // Извлечь текущие настройки для сохранения
  void extractSettings(s21::Settings& settings) const;

  // --- Параметры отображения (используются как внутреннее состояние или в GUI)
  // ---
  float line_width = 1.0f;   // Толщина ребер
  float point_width = 1.0f;  // Размер вершин

  // Тип линии, точек, проекции
  enum LineType { SOLID, DASHED } type_line = SOLID;
  enum PointType { NONE, SQUARE, CIRCLE } type_point = SQUARE;
  enum ProjectionType { PARALLEL, CENTRAL } type_projection = PARALLEL;

  float color_line[3] = {1.0f, 1.0f, 1.0f};       // Цвет рёбер
  float color_point[3] = {1.0f, 0.0f, 0.0f};      // Цвет вершин
  float color_fon[4] = {0.0f, 0.0f, 0.0f, 1.0f};  // Цвет фона (RGBA)

  // --- Параметры масштабирования ---
  float kMinScale = 0.1f;  // Минимальный масштаб
  float last_scale_ = 1.0f;  // Последнее значение масштаба
  float kMaxScale = 100.0f;  // Максимальный масштаб
  int kSliderSteps = 100;    // Шаги слайдера масштаба

  // --- Параметры перемещения ---
  float last_move_x_ = 0.0f;
  float last_move_y_ = 0.0f;
  float last_move_z_ = 0.0f;

  float move_min = -5.0f;  // Минимальное перемещение
  float move_max = 5.0f;   // Максимальное перемещение
  int move_slider_steps = 255;  // Шаги слайдера перемещения

  // --- Константы для записи GIF ---
  static constexpr int kGifWidth = 640;
  static constexpr int kGifHeight = 480;
  static constexpr int kGifFPS = 10;
  static constexpr int kGifDurationSeconds = 5;
  static constexpr int kGifFrameCount = kGifFPS * kGifDurationSeconds;

  // --- Слайдеры управления из интерфейса ---
  QSlider* scaleSlider_ = nullptr;

  QSlider* slider_move_x_ = nullptr;
  QSlider* slider_move_y_ = nullptr;
  QSlider* slider_move_z_ = nullptr;

  // --- Повороты через слайдер и мышь ---
  float rotation_x_from_slider_ = 0.0f;
  float rotation_x_from_mouse_ = 0.0f;

  float rotation_y_from_slider_ = 0.0f;
  float rotation_y_from_mouse_ = 0.0f;

  float rotation_z_from_slider_ = 0.0f;
  float rotation_z_from_mouse_ = 0.0f;

  QSlider* rotateSliderX_ = nullptr;
  QSlider* rotateSliderY_ = nullptr;
  QSlider* rotateSliderZ_ = nullptr;

  // --- Виджеты с количеством вершин и рёбер ---
  QLabel* vertices_count_ = nullptr;
  QLabel* edges_count_ = nullptr;
  QLabel* filename_label_ = nullptr;

  // --- Методы связи с UI ---
  void openModelFile();  // Открытие OBJ-файла

  void setScaleSlider(QSlider* slider);  // Связь слайдера масштаба
  void setSliderMoveX(QSlider* slider);  // Связь слайдера перемещения X
  void setSliderMoveY(QSlider* slider);  // Связь слайдера перемещения Y
  void setSliderMoveZ(QSlider* slider);  // Связь слайдера перемещения Z

  void setRotateSliders(QSlider* x, QSlider* y, QSlider* z);  // Поворот

  void setVertexLcd(QLabel* lcd);  // Установка виджета для числа вершин
  void setEdgesLcd(QLabel* lcd);  // Установка виджета для числа рёбер
  void setFilenameLabel(QLabel* label);  // Установка подписи имени файла

 protected:
  // --- Переопределённые методы от QOpenGLWidget ---
  void initializeGL() override;  // Инициализация OpenGL
  void paintGL() override;  // Основная отрисовка сцены

  void mousePressEvent(QMouseEvent* event) override;  // Обработка нажатия мыши
  void mouseMoveEvent(
      QMouseEvent* event) override;  // Обработка перемещения мыши
  void wheelEvent(QWheelEvent* event) override;  // Обработка колеса мыши

 public slots:
  // --- Слоты для управления из GUI ---
  void onScaleSliderChanged(int value);  // Масштаб
  void gif_figure();               // Отрисовка кадра для gif
  void onMoveXChanged(int value);  // Перемещение по X
  void onMoveYChanged(int value);  // Перемещение по Y
  void onMoveZChanged(int value);  // Перемещение по Z

  void onRotateXChanged(int value);  // Вращение по X
  void onRotateYChanged(int value);  // Вращение по Y
  void onRotateZChanged(int value);  // Вращение по Z

  void chooseBackgroundColor();  // Выбор цвета фона
  void setParallelProjection();  // Параллельная проекция
  void setCentralProjection();   // Центральная проекция

  void setSolidLine();   // Сплошные линии
  void setDashedLine();  // Пунктирные линии
  // void setEdgeWidth(int width);          // Установка толщины линии
  float getEdgeWidth() const;  // Получение толщины линии

  void chooseEdgeColor();  // Выбор цвета рёбер

  void setNoneVertexType();    // Отключение вершин
  void setCircleVertexType();  // Круглые вершины
  void setSquareVertexType();  // Квадратные вершины

  void chooseVertexColor();  // Выбор цвета вершин
  void setEdgeWidth(int value);
  void setVertexSize(int size);  // Установка размера вершин

  void saveAsJPEG();         // Сохранение сцены как JPEG
  void saveAsBMP();          // Сохранение сцены как BMP
  void startGifRecording();  // Начало записи gif

 private:
  // --- Приватные методы отрисовки и вспомогательные функции ---
  void drawEdges(const s21::Mesh& mesh);     // Отрисовка рёбер
  void drawVertices(const s21::Mesh& mesh);  // Отрисовка вершин
  void setBackgroundColor();  // Применение цвета фона
  void setProjection();  // Установка матрицы проекции
  float cosDistance(float* fst, float* snd);  // Расчет угла между векторами
  void create_gif();                          // Генерация gif

  // --- Вспомогательные поля ---
  QTimer* timer = nullptr;            // Таймер для gif
  int num = 0;                        // Счётчик кадров gif
  QImage screencast[kGifFrameCount];  // Буфер для скриншотов
  QPoint mouse_position;              // Позиция мыши

  QImage gifFrames[kGifFrameCount];  // Кадры gif
  int gifCurrentFrame = 0;           // Текущий кадр gif

  s21::ModelFacade* facade_ = nullptr;  // Указатель на фасад модели
};

#endif  // OPENGL_IMPL_H