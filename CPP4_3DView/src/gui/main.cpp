#include <QApplication>

#include "../model/s21_Facade.hh"
#include "controller.hh"
#include "mainwindow.h"
#include "ui_mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  s21::Controller controller;
  MainWindow w(controller);
  w.show();

  return a.exec();
}
