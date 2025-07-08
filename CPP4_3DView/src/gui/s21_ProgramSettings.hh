#ifndef S21_PROGRAM_SETTINGS_LIB
#define S21_PROGRAM_SETTINGS_LIB

#include <QColor>
#include <QSettings>
#include <QString>
#include <QVariant>

#include "../commons/s21_objects.hh"

namespace s21 {

enum class ProjectionType { Parallel, Central };

struct VertexStyle {
  enum class DisplayType { None, Circle, Square };

  QColor vertexColor;

  double vertexSize;

  DisplayType displayType;

  VertexStyle(QColor vertexColor = QColor(0.0f, 0.0f, 0.0f),
              double vertexSize = 1.0,
              DisplayType displayType = DisplayType::None)
      : vertexColor(vertexColor),
        vertexSize(vertexSize),
        displayType(displayType) {}
};

struct EdgeStyle {
  enum class DisplayType { Solid, Dotted };

  QColor edgeColor;

  double edgeThickness;

  DisplayType displayType;

  EdgeStyle(QColor edgeColor = QColor(0.0f, 0.0f, 0.0f),
            double edgeThickness = 1.0,
            DisplayType displayType = DisplayType::Solid)
      : edgeColor(edgeColor),
        edgeThickness(edgeThickness),
        displayType(displayType) {}
};

class Settings {
  QColor backgroundColor;

  ProjectionType projectionType;

  EdgeStyle edgeStyle;

  VertexStyle vertexStyle;

 public:
  Settings(QColor backgroundColor = QColor(0.0f, 0.0f, 0.0f),
           ProjectionType ProjectionType = ProjectionType::Parallel,
           EdgeStyle edgeStyle = EdgeStyle(),
           VertexStyle vertexStyle = VertexStyle())
      : backgroundColor(backgroundColor),
        projectionType(ProjectionType),
        edgeStyle(edgeStyle),
        vertexStyle(vertexStyle) {}

  void saveToFile(const std::string& filePath) const;
  void loadFromFile(const std::string& filePath);

  QColor getBackgroundColor() const { return backgroundColor; }
  ProjectionType getProjectionType() const { return projectionType; }
  EdgeStyle getEdgeStyle() const { return edgeStyle; }
  VertexStyle getVertexStyle() const { return vertexStyle; }

  void setProjectionType(ProjectionType type) { projectionType = type; }
  void setEdgeStyle(const EdgeStyle& style) { edgeStyle = style; }
  void setVertexStyle(const VertexStyle& style) { vertexStyle = style; }
  void setBackgroundColor(const QColor& color) { backgroundColor = color; }
};

}  // namespace s21

#endif