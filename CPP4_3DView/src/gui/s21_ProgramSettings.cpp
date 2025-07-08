#include "s21_ProgramSettings.hh"

namespace s21 {

void Settings::saveToFile(const std::string& filePath) const {
  QSettings settings(QString::fromStdString(filePath), QSettings::IniFormat);

  settings.beginGroup("Application");
  settings.setValue("backgroundColor", backgroundColor.name(QColor::HexArgb));
  settings.setValue("projectionType", static_cast<int>(projectionType));

  settings.beginGroup("EdgeStyle");
  settings.setValue("edgeColor", edgeStyle.edgeColor);
  settings.setValue("edgeThickness", edgeStyle.edgeThickness);
  settings.setValue("displayType", static_cast<int>(edgeStyle.displayType));
  settings.endGroup();

  settings.beginGroup("VertexStyle");
  settings.setValue("vertexColor", vertexStyle.vertexColor);
  settings.setValue("vertexSize", vertexStyle.vertexSize);
  settings.setValue("displayType", static_cast<int>(vertexStyle.displayType));
  settings.endGroup();

  settings.endGroup();
}

void Settings::loadFromFile(const std::string& filePath) {
  QSettings settings(QString::fromStdString(filePath), QSettings::IniFormat);

  settings.beginGroup("Application");

  QString colorStr = settings.value("backgroundColor", "#FF000000").toString();
  backgroundColor = QColor(colorStr);

  projectionType = static_cast<ProjectionType>(
      settings
          .value("projectionType", static_cast<int>(ProjectionType::Parallel))
          .toInt());

  settings.beginGroup("EdgeStyle");
  edgeStyle.edgeColor =
      settings.value("edgeColor", QColor(0, 0, 0)).value<QColor>();
  edgeStyle.edgeThickness = settings.value("edgeThickness", 1.0).toDouble();
  edgeStyle.displayType = static_cast<EdgeStyle::DisplayType>(
      settings
          .value("displayType", static_cast<int>(EdgeStyle::DisplayType::Solid))
          .toInt());
  settings.endGroup();

  settings.beginGroup("VertexStyle");
  vertexStyle.vertexColor =
      settings.value("vertexColor", QColor(0, 0, 0)).value<QColor>();
  vertexStyle.vertexSize = settings.value("vertexSize", 1.0).toDouble();
  vertexStyle.displayType = static_cast<VertexStyle::DisplayType>(
      settings
          .value("displayType",
                 static_cast<int>(VertexStyle::DisplayType::None))
          .toInt());
  settings.endGroup();

  settings.endGroup();
}

}  // namespace s21