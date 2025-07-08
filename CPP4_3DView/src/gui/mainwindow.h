#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QTimer>
#include <QtOpenGL>

#include "controller.hh"
#include "openglimpl.h"
#include "s21_ProgramSettings.hh"

using s21::Settings;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public s21::IObserver {
  Q_OBJECT

 public:
  const QString SETTINGS_FILE = "settings.ini";

  MainWindow(s21::Controller &controller, QWidget *parent = nullptr);
  ~MainWindow();

  void Update() override;
  void loadSettingsFromFile(const QString &SETTINGS_FILE);
  void saveSettingsToFile(const QString &SETTINGS_FILE);

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::MainWindow *ui;
  OpenGLImpl *glwidget;
  s21::Settings settings;
  void setupUiElements();
};
#endif  // MAINWINDOW_H
