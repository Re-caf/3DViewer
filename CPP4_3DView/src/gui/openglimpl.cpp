#include "openglimpl.h"

OpenGLImpl::OpenGLImpl(QWidget* parent) : QOpenGLWidget(parent) {
  setWindowTitle("3DViewer_v2.0");

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(gif_figure()));
}

OpenGLImpl::~OpenGLImpl() {}

void OpenGLImpl::setModelFacade(s21::ModelFacade* facade) { facade_ = facade; }

void OpenGLImpl::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void OpenGLImpl::paintGL() {
  setBackgroundColor();
  setProjection();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (facade_ && facade_->hasModel()) {
    s21::Mesh mesh = facade_->getMesh();
    drawEdges(mesh);
    drawVertices(mesh);
  }
}

void OpenGLImpl::setBackgroundColor() {
  glClearColor(color_fon[0], color_fon[1], color_fon[2], color_fon[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLImpl::setProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  switch (type_projection) {
    case PARALLEL:
      glOrtho(-3, 3, -3, 3, -4, 4);
      break;
    case CENTRAL:
      glFrustum(-2, 2, -2, 2, 2, 6);
      glTranslatef(0, 0, -3);
      break;
  }
}

void OpenGLImpl::drawEdges(const s21::Mesh& mesh) {
  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3f(color_line[0], color_line[1], color_line[2]);
  glLineWidth(line_width);

  std::vector<float> vertex_coords;
  vertex_coords.reserve(mesh.vertices.size() * 3);

  for (const auto& v : mesh.vertices) {
    vertex_coords.push_back(v.position[0]);
    vertex_coords.push_back(v.position[1]);
    vertex_coords.push_back(v.position[2]);
  }

  glVertexPointer(3, GL_FLOAT, 0, vertex_coords.data());

  switch (type_line) {
    case SOLID:
      for (size_t i = 0; i + 1 < mesh.indices.size(); i += 2) {
        glBegin(GL_LINES);
        glArrayElement(mesh.indices[i]);
        glArrayElement(mesh.indices[i + 1]);
        glEnd();
      }
      break;
    case DASHED:
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(2, 0x00FF);
      for (size_t i = 0; i + 1 < mesh.indices.size(); i += 2) {
        glBegin(GL_LINES);
        glArrayElement(mesh.indices[i]);
        glArrayElement(mesh.indices[i + 1]);
        glEnd();
      }
      glDisable(GL_LINE_STIPPLE);
      break;
  }

  glDisableClientState(GL_VERTEX_ARRAY);
}

void OpenGLImpl::drawVertices(const s21::Mesh& mesh) {
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnable(GL_DEPTH_TEST);
  glColor3f(color_point[0], color_point[1], color_point[2]);
  glPointSize(point_width);

  std::vector<float> vertex_coords;
  vertex_coords.reserve(mesh.vertices.size() * 3);
  for (const auto& v : mesh.vertices) {
    vertex_coords.push_back(v.position[0]);
    vertex_coords.push_back(v.position[1]);
    vertex_coords.push_back(v.position[2]);
  }

  glVertexPointer(3, GL_FLOAT, 0, vertex_coords.data());

  switch (type_point) {
    case NONE:
      break;
    case SQUARE:
      glDrawArrays(GL_POINTS, 0, mesh.vertexCount());
      break;
    case CIRCLE:
      glEnable(GL_POINT_SMOOTH);
      glDrawArrays(GL_POINTS, 0, mesh.vertexCount());
      glDisable(GL_POINT_SMOOTH);
      break;
  }

  glDisableClientState(GL_VERTEX_ARRAY);
}

float OpenGLImpl::cosDistance(float* fst, float* snd) {
  float sum = 0, len_first = 0, len_second = 0;
  for (int i = 0; i < 3; i++) {
    sum += fst[i] * snd[i];
    len_first += fst[i] * fst[i];
    len_second += snd[i] * snd[i];
  }
  return sum / (std::sqrt(len_first) * std::sqrt(len_second));
}

void OpenGLImpl::mousePressEvent(QMouseEvent* event) {
  mouse_position = event->pos();
}

void OpenGLImpl::mouseMoveEvent(QMouseEvent* event) {
  if (!(event->buttons() & Qt::LeftButton)) return;

  float delta_x = (event->pos().x() - mouse_position.x()) * 0.001f;
  float delta_y = (event->pos().y() - mouse_position.y()) * 0.001f;

  float new_rotation_x =
      rotation_x_from_slider_ + rotation_x_from_mouse_ - delta_y;
  float new_rotation_y =
      rotation_y_from_slider_ + rotation_y_from_mouse_ + delta_x;

  // Ограничения: -3.60f .. 3.60f
  constexpr float kRotationLimit = 3.60f;
  new_rotation_x = std::clamp(new_rotation_x, -kRotationLimit, kRotationLimit);
  new_rotation_y = std::clamp(new_rotation_y, -kRotationLimit, kRotationLimit);

  // Дельты с учётом ограничений
  float applied_delta_x =
      new_rotation_x - (rotation_x_from_slider_ + rotation_x_from_mouse_);
  float applied_delta_y =
      new_rotation_y - (rotation_y_from_slider_ + rotation_y_from_mouse_);

  try {
    if (facade_) {
      facade_->applyRotation(applied_delta_x, applied_delta_y, 0.0f);
    }

    rotation_x_from_mouse_ += applied_delta_x;
    rotation_y_from_mouse_ += applied_delta_y;

    // Синхронизация слайдеров
    if (rotateSliderX_) {
      int x_value = static_cast<int>(
          (rotation_x_from_slider_ + rotation_x_from_mouse_) * 100);
      x_value = std::clamp(x_value, rotateSliderX_->minimum(),
                           rotateSliderX_->maximum());
      QSignalBlocker blocker(rotateSliderX_);
      rotateSliderX_->setValue(x_value);
    }

    if (rotateSliderY_) {
      int y_value = static_cast<int>(
          (rotation_y_from_slider_ + rotation_y_from_mouse_) * 100);
      y_value = std::clamp(y_value, rotateSliderY_->minimum(),
                           rotateSliderY_->maximum());
      QSignalBlocker blocker(rotateSliderY_);
      rotateSliderY_->setValue(y_value);
    }

    update();
  } catch (const std::exception& e) {
    QMessageBox::critical(this, "Mouse Rotation Error", e.what());
  }

  mouse_position = event->pos();
}

void OpenGLImpl::screenWidget(int index) {
  QImage image = grabFramebuffer();
  QString path = QFileDialog::getSaveFileName(this, "Save as", "",
                                              index == 0 ? "*.jpg" : "*.bmp");
  image.save(path, index == 0 ? "JPEG" : "BMP");
}

void OpenGLImpl::startGifRecording() {
  gifCurrentFrame = 0;

  // Настроим таймер на интервал 1000 / fps
  timer->start(1000 / kGifFPS);
}

void OpenGLImpl::gif_figure() {
  if (gifCurrentFrame >= kGifFrameCount) {
    timer->stop();
    create_gif();
    return;
  }

  // Без чёрных полос — масштабируем жёстко под 640x480
  QImage frame = grabFramebuffer().scaled(
      kGifWidth, kGifHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  gifFrames[gifCurrentFrame++] = frame;
}

void OpenGLImpl::create_gif() {
  QGifImage gif(QSize(kGifWidth, kGifHeight));
  gif.setDefaultDelay(1000 / kGifFPS);  // задержка в мс

  for (int i = 0; i < kGifFrameCount; ++i) {
    gif.addFrame(gifFrames[i]);
  }

  QString path =
      QFileDialog::getSaveFileName(this, "Save GIF", "", "GIF Image (*.gif)");
  if (!path.isEmpty()) {
    gif.save(path);
  }
}

void OpenGLImpl::openModelFile() {
  QString filePath = QFileDialog::getOpenFileName(
      this, "Open Model", "", "OBJ Files (*.obj);;All Files (*)");
  if (!filePath.isEmpty() && facade_) {
    try {
      facade_->loadModel(filePath.toStdString());
      last_scale_ = 1.0f;

      last_move_x_ = 0.0f;
      last_move_y_ = 0.0f;
      last_move_z_ = 0.0f;

      if (filename_label_) {
        filename_label_->setText(
            QString::fromStdString(facade_->getFilename()));
      }

      if (vertices_count_) {
        vertices_count_->setText(
            QString::number(facade_->getCountVertexFacade()));
      }

      if (edges_count_) {
        edges_count_->setText(QString::number(facade_->getCountEdgesFacade()));
      }

      if (slider_move_x_) {
        slider_move_x_->setValue(move_slider_steps / 2);  // середина шкалы
      }

      if (slider_move_y_) {
        slider_move_y_->setValue(move_slider_steps / 2);  // середина шкалы
      }

      if (slider_move_z_) {
        slider_move_z_->setValue(move_slider_steps / 2);  // середина шкалы
      }

      if (scaleSlider_) {
        scaleSlider_->setValue(last_scale_);
      }

      if (rotateSliderX_) {
        QSignalBlocker blocker(rotateSliderX_);
        rotateSliderX_->setValue(0);
      }

      if (rotateSliderY_) {
        QSignalBlocker blocker(rotateSliderY_);
        rotateSliderY_->setValue(0);
      }

      if (rotateSliderZ_) {
        QSignalBlocker blocker(rotateSliderZ_);
        rotateSliderZ_->setValue(0);
      }

      update();
    } catch (const std::exception& e) {
      QMessageBox::critical(this, "Error",
                            QString("Failed to load model:\n%1").arg(e.what()));
    }
  }
}

void OpenGLImpl::onScaleSliderChanged(int value) {
  if (facade_) {
    float new_scale = kMinScale + (static_cast<float>(value) / kSliderSteps) *
                                      (kMaxScale - kMinScale);
    new_scale = std::clamp(new_scale, kMinScale, kMaxScale);

    float relative_scale = new_scale / last_scale_;
    if (relative_scale != 1.0f) {
      last_scale_ = new_scale;

      try {
        facade_->applyScaling(relative_scale);
        update();
      } catch (const std::exception& e) {
        QMessageBox::critical(this, "Scaling Error", e.what());
      }
    }
  }
}

void OpenGLImpl::wheelEvent(QWheelEvent* event) {
  if (facade_) {
    constexpr float scaleStep = 1.1f;
    float factor = event->angleDelta().y() > 0 ? scaleStep : 1.0f / scaleStep;

    float new_scale = last_scale_ * factor;
    new_scale = std::clamp(new_scale, kMinScale, kMaxScale);

    factor = new_scale / last_scale_;
    if (factor != 1.0f) {
      try {
        facade_->applyScaling(factor);
        last_scale_ = new_scale;
        update();

        if (scaleSlider_) {
          int slider_value = static_cast<int>(
              ((last_scale_ - kMinScale) / (kMaxScale - kMinScale)) *
              kSliderSteps);
          slider_value = std::clamp(slider_value, 0, kSliderSteps);
          scaleSlider_->setValue(slider_value);
        }

      } catch (const std::exception& e) {
        QMessageBox::critical(this, "Scaling Error", e.what());
      }
    }
  }
}

void OpenGLImpl::setScaleSlider(QSlider* slider) {
  scaleSlider_ = slider;

  if (scaleSlider_) {
    int value = scaleSlider_->value();
    last_scale_ = kMinScale + (static_cast<float>(value) / kSliderSteps) *
                                  (kMaxScale - kMinScale);
    last_scale_ = std::clamp(last_scale_, kMinScale, kMaxScale);
  }
}

void OpenGLImpl::onMoveXChanged(int value) {
  if (facade_) {
    // Преобразуем значение слайдера в диапазон [-max, +max]
    float new_x = move_min + (static_cast<float>(value) / move_slider_steps) *
                                 (move_max - move_min);
    new_x = std::clamp(new_x, move_min, move_max);

    float delta_x = new_x - last_move_x_;
    if (delta_x != 0.0f) {
      last_move_x_ = new_x;

      try {
        facade_->applyTranslation(delta_x, 0.0f, 0.0f);
        update();
      } catch (const std::exception& e) {
        QMessageBox::critical(this, "Translation Error", e.what());
      }
    }
  }
}

void OpenGLImpl::setSliderMoveX(QSlider* slider) {
  slider_move_x_ = slider;

  if (slider_move_x_) {
    slider_move_x_->setValue(move_slider_steps / 2);
  }
}

void OpenGLImpl::setSliderMoveY(QSlider* slider) {
  slider_move_y_ = slider;

  if (slider_move_y_) {
    slider_move_y_->setValue(move_slider_steps / 2);
  }
}

void OpenGLImpl::setSliderMoveZ(QSlider* slider) {
  slider_move_z_ = slider;

  if (slider_move_z_) {
    slider_move_z_->setValue(move_slider_steps / 2);
  }
}

void OpenGLImpl::onMoveYChanged(int value) {
  if (facade_) {
    // Преобразуем значение слайдера в диапазон [-max, +max]
    float new_y = move_min + (static_cast<float>(value) / move_slider_steps) *
                                 (move_max - move_min);
    new_y = std::clamp(new_y, move_min, move_max);

    float delta_y = new_y - last_move_y_;
    if (delta_y != 0.0f) {
      last_move_y_ = new_y;

      try {
        facade_->applyTranslation(0.0f, delta_y, 0.0f);
        update();
      } catch (const std::exception& e) {
        QMessageBox::critical(this, "Translation Error", e.what());
      }
    }
  }
}

void OpenGLImpl::onMoveZChanged(int value) {
  if (facade_) {
    // Преобразуем значение слайдера в диапазон [-max, +max]
    float new_z = move_min + (static_cast<float>(value) / move_slider_steps) *
                                 (move_max - move_min);
    new_z = std::clamp(new_z, move_min, move_max);

    float delta_z = new_z - last_move_z_;
    if (delta_z != 0.0f) {
      last_move_z_ = new_z;

      try {
        facade_->applyTranslation(0.0f, 0.0f, delta_z);
        update();
      } catch (const std::exception& e) {
        QMessageBox::critical(this, "Translation Error", e.what());
      }
    }
  }
}

void OpenGLImpl::onRotateXChanged(int value) {
  if (!facade_) return;

  float new_rotation = value * 0.01f;
  float delta =
      new_rotation - (rotation_x_from_slider_ + rotation_x_from_mouse_);

  if (std::abs(delta) > std::numeric_limits<float>::epsilon()) {
    try {
      facade_->applyRotation(delta, 0.0f, 0.0f);
      rotation_x_from_slider_ = new_rotation;
      rotation_x_from_mouse_ = 0.0f;
      update();
    } catch (const std::exception& e) {
      QMessageBox::critical(this, "Rotation X Error", e.what());
    }
  }
}

void OpenGLImpl::onRotateYChanged(int value) {
  if (!facade_) return;

  float new_rotation = value * 0.01f;
  float delta =
      new_rotation - (rotation_y_from_slider_ + rotation_y_from_mouse_);

  if (std::abs(delta) > std::numeric_limits<float>::epsilon()) {
    try {
      facade_->applyRotation(0.0f, delta, 0.0f);
      rotation_y_from_slider_ = new_rotation;
      rotation_y_from_mouse_ = 0.0f;
      update();
    } catch (const std::exception& e) {
      QMessageBox::critical(this, "Rotation Y Error", e.what());
    }
  }
}

void OpenGLImpl::onRotateZChanged(int value) {
  if (!facade_) return;

  float new_rotation = value * 0.01f;
  float delta =
      new_rotation - (rotation_z_from_slider_ + rotation_z_from_mouse_);

  if (std::abs(delta) > std::numeric_limits<float>::epsilon()) {
    try {
      facade_->applyRotation(0.0f, 0.0f, delta);
      rotation_z_from_slider_ = new_rotation;
      rotation_z_from_mouse_ = 0.0f;
      update();
    } catch (const std::exception& e) {
      QMessageBox::critical(this, "Rotation Z Error", e.what());
    }
  }
}

void OpenGLImpl::setRotateSliders(QSlider* x, QSlider* y, QSlider* z) {
  rotateSliderX_ = x;
  rotateSliderY_ = y;
  rotateSliderZ_ = z;

  if (rotateSliderX_) rotation_x_from_slider_ = rotateSliderX_->value() * 0.01f;
  if (rotateSliderY_) rotation_y_from_slider_ = rotateSliderY_->value() * 0.01f;
  if (rotateSliderZ_) rotation_z_from_slider_ = rotateSliderZ_->value() * 0.01f;
}

void OpenGLImpl::setVertexLcd(QLabel* lcd) { vertices_count_ = lcd; }

void OpenGLImpl::setEdgesLcd(QLabel* lcd) { edges_count_ = lcd; }

void OpenGLImpl::setFilenameLabel(QLabel* label) { filename_label_ = label; }

void OpenGLImpl::chooseBackgroundColor() {
  QColor color =
      QColorDialog::getColor(Qt::black, this, "Choose Background Color");
  if (color.isValid()) {
    color_fon[0] = color.redF();
    color_fon[1] = color.greenF();
    color_fon[2] = color.blueF();
    color_fon[3] = 1.0f;  // Прозрачность фиксированная

    update();  // Перерисовка сцены с новым фоном
  }
}

void OpenGLImpl::setParallelProjection() {
  type_projection = PARALLEL;
  setProjection();
  update();
}

void OpenGLImpl::setCentralProjection() {
  type_projection = CENTRAL;
  setProjection();
  update();
}

void OpenGLImpl::setSolidLine() {
  type_line = SOLID;
  update();  // Перерисовать сцену
}

void OpenGLImpl::setDashedLine() {
  type_line = DASHED;
  update();  // Перерисовать сцену
}

void OpenGLImpl::setEdgeWidth(int width) {
  line_width = static_cast<float>(width);  // Преобразуем к float
  update();  // Перерисовать сцену
}

float OpenGLImpl::getEdgeWidth() const { return line_width; }

void OpenGLImpl::chooseEdgeColor() {
  QColor color =
      QColorDialog::getColor(Qt::white, this, "Выберите цвет граней");

  if (color.isValid()) {
    color_line[0] = color.redF();  // redF() возвращает float [0.0–1.0]
    color_line[1] = color.greenF();
    color_line[2] = color.blueF();
    update();  // Перерисовать сцену
  }
}

void OpenGLImpl::setNoneVertexType() {
  type_point = NONE;
  update();  // Перерисовать сцену
}

void OpenGLImpl::setCircleVertexType() {
  type_point = CIRCLE;
  update();  // Перерисовать сцену
}

void OpenGLImpl::setSquareVertexType() {
  type_point = SQUARE;
  update();  // Перерисовать сцену
}

void OpenGLImpl::chooseVertexColor() {
  QColor color =
      QColorDialog::getColor(Qt::white, this, "Выберите цвет вершин");

  if (color.isValid()) {
    color_point[0] = color.redF();  // значения от 0.0 до 1.0
    color_point[1] = color.greenF();
    color_point[2] = color.blueF();
    update();  // Перерисовать сцену
  }
}

void OpenGLImpl::setVertexSize(int size) {
  point_width = static_cast<float>(size);  // преобразуем int в float
  update();  // Перерисовать сцену
}

// void OpenGLImpl::setEdgeWidth(int size) {
//   line_width = static_cast<float>(size);  // преобразуем int в float
//   update();  // Перерисовать сцену
// }

void OpenGLImpl::applySettings(const s21::Settings& settings) {
  type_projection = static_cast<ProjectionType>(settings.getProjectionType());

  color_line[0] = settings.getEdgeStyle().edgeColor.redF();
  color_line[1] = settings.getEdgeStyle().edgeColor.greenF();
  color_line[2] = settings.getEdgeStyle().edgeColor.blueF();
  line_width = settings.getEdgeStyle().edgeThickness;
  type_line = static_cast<LineType>(settings.getEdgeStyle().displayType);

  color_point[0] = settings.getVertexStyle().vertexColor.redF();
  color_point[1] = settings.getVertexStyle().vertexColor.greenF();
  color_point[2] = settings.getVertexStyle().vertexColor.blueF();
  point_width = settings.getVertexStyle().vertexSize;
  type_point = static_cast<PointType>(settings.getVertexStyle().displayType);

  color_fon[0] = settings.getBackgroundColor().redF();
  color_fon[1] = settings.getBackgroundColor().greenF();
  color_fon[2] = settings.getBackgroundColor().blueF();
  color_fon[3] = settings.getBackgroundColor().alphaF();

  update();  // перерисовать сцену
}

void OpenGLImpl::extractSettings(s21::Settings& settings) const {
  settings.setProjectionType(static_cast<s21::ProjectionType>(type_projection));

  s21::EdgeStyle edgeStyle;
  edgeStyle.edgeColor =
      QColor::fromRgbF(color_line[0], color_line[1], color_line[2]);
  edgeStyle.edgeThickness = line_width;
  edgeStyle.displayType = static_cast<s21::EdgeStyle::DisplayType>(type_line);
  settings.setEdgeStyle(edgeStyle);

  s21::VertexStyle vertexStyle;
  vertexStyle.vertexColor =
      QColor::fromRgbF(color_point[0], color_point[1], color_point[2]);
  vertexStyle.vertexSize = point_width;
  vertexStyle.displayType =
      static_cast<s21::VertexStyle::DisplayType>(type_point);
  settings.setVertexStyle(vertexStyle);

  settings.setBackgroundColor(
      QColor::fromRgbF(color_fon[0], color_fon[1], color_fon[2], color_fon[3]));
}

void OpenGLImpl::saveAsJPEG() {
  QImage image = this->grabFramebuffer();  // Снимок текущего OpenGL-сцены

  QString fileName = QFileDialog::getSaveFileName(this, "Save as JPEG", "",
                                                  "JPEG Image (*.jpg *.jpeg)");

  if (!fileName.isEmpty()) {
    image.save(fileName, "JPEG");
  }
}

void OpenGLImpl::saveAsBMP() {
  QImage image =
      this->grabFramebuffer();  // Захватываем содержимое OpenGL-буфера

  QString fileName = QFileDialog::getSaveFileName(this, "Save as BMP", "",
                                                  "Bitmap Image (*.bmp)");

  if (!fileName.isEmpty()) {
    image.save(fileName, "BMP");
  }
}
