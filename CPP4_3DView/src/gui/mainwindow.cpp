#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(s21::Controller &controller, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->setFixedSize(this->size());

  glwidget = new OpenGLImpl(this);

  glwidget->setModelFacade(&controller.GetModelFacade());

  ui->openGLlayout->addWidget(glwidget);

  setupUiElements();

  loadSettingsFromFile(SETTINGS_FILE);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::Update() { glwidget->update(); }

void MainWindow::loadSettingsFromFile(const QString &filePath) {
  if (!QFile::exists(filePath)) return;
  s21::Settings settings;
  settings.loadFromFile(filePath.toStdString());
  glwidget->applySettings(settings);
}

void MainWindow::saveSettingsToFile(const QString &filePath) {
  s21::Settings settings;
  glwidget->extractSettings(settings);
  settings.saveToFile(filePath.toStdString());
}

void MainWindow::closeEvent(QCloseEvent *event) {
  saveSettingsToFile(SETTINGS_FILE);
  event->accept();
}

void MainWindow::setupUiElements() {
  connect(ui->fileDialogButton, &QPushButton::clicked, glwidget,
          &OpenGLImpl::openModelFile);

  // Масштаб
  glwidget->setScaleSlider(ui->slider_scale);
  ui->slider_scale->setRange(0, 100);
  ui->slider_scale->setValue(0);
  connect(ui->slider_scale, &QSlider::valueChanged, glwidget,
          &OpenGLImpl::onScaleSliderChanged);

  // Перемещение
  glwidget->setSliderMoveX(ui->slider_move_x);
  glwidget->setSliderMoveY(ui->slider_move_y);
  glwidget->setSliderMoveZ(ui->slider_move_z);

  ui->slider_move_x->setValue(255 / 2);
  ui->slider_move_y->setValue(255 / 2);
  ui->slider_move_z->setValue(255 / 2);

  connect(ui->slider_move_x, &QSlider::valueChanged, glwidget,
          &OpenGLImpl::onMoveXChanged);
  connect(ui->slider_move_y, &QSlider::valueChanged, glwidget,
          &OpenGLImpl::onMoveYChanged);
  connect(ui->slider_move_z, &QSlider::valueChanged, glwidget,
          &OpenGLImpl::onMoveZChanged);

  // Поворот
  glwidget->setRotateSliders(ui->slider_rotate_x, ui->slider_rotate_y,
                             ui->slider_rotate_z);

  ui->slider_rotate_x->setRange(-360, 360);
  ui->slider_rotate_y->setRange(-360, 360);
  ui->slider_rotate_z->setRange(-360, 360);

  connect(ui->slider_rotate_x, &QSlider::valueChanged, glwidget,
          &OpenGLImpl::onRotateXChanged);
  connect(ui->slider_rotate_y, &QSlider::valueChanged, glwidget,
          &OpenGLImpl::onRotateYChanged);
  connect(ui->slider_rotate_z, &QSlider::valueChanged, glwidget,
          &OpenGLImpl::onRotateZChanged);

  // Метки
  glwidget->setVertexLcd(ui->vertices_count);
  glwidget->setEdgesLcd(ui->edges_count);
  glwidget->setFilenameLabel(ui->filename_label);

  // Цвет фона
  connect(ui->background_color, &QPushButton::clicked, glwidget,
          &OpenGLImpl::chooseBackgroundColor);

  // Проекции
  connect(ui->parallel_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::setParallelProjection);
  connect(ui->central_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::setCentralProjection);

  // Тип линии
  connect(ui->solid_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::setSolidLine);
  connect(ui->dashed_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::setDashedLine);

  // Толщина рёбер
  settings.loadFromFile("settings.ini");
  ui->slider_edges->setRange(1, 10);
  int edgeWidth = static_cast<int>(settings.getEdgeStyle().edgeThickness);
  ui->slider_edges->setValue(edgeWidth);

  connect(ui->slider_edges, &QSlider::valueChanged, glwidget,
          &OpenGLImpl::setEdgeWidth);
  connect(ui->edges_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::chooseEdgeColor);

  // Вершины
  connect(ui->none_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::setNoneVertexType);
  connect(ui->circle_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::setCircleVertexType);
  connect(ui->square_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::setSquareVertexType);

  connect(ui->vertices_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::chooseVertexColor);

  ui->slider_vertices->setRange(1, 25);
  int vertexSize = static_cast<int>(settings.getVertexStyle().vertexSize);
  ui->slider_vertices->setValue(vertexSize);

  connect(ui->slider_vertices, &QSlider::valueChanged, glwidget,
          &OpenGLImpl::setVertexSize);

  // Сохранение изображений
  connect(ui->jpeg_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::saveAsJPEG);
  connect(ui->bmp_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::saveAsBMP);
  connect(ui->gif_button, &QPushButton::clicked, glwidget,
          &OpenGLImpl::startGifRecording);
}
