#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QDir>
#include <QFileSystemModel>
#include <QMainWindow>
#include <QStringList>
#include <QWidget>
#include <QtGui>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "glview.h"
#include "qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

extern "C" {}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_listView_doubleClicked(const QModelIndex &index);
  void on_pushButton_clicked();
  void setColor();
  void proectionChanged();
  void dotTypeChanged();
  void lineTypeChanged();
  void radioButtonValue();

  void on_verticalScrollBar_valueChanged(int value);

  void slotTimerEnd();
  void on_pushButtonRec_clicked();

 public:
  vertex points{};
  object model{};

 private:
  void setStyles();
  void settingConfig();
  Ui::MainWindow *ui;
  glView *win;
  QFileSystemModel *fileList;
  QDir dir;
  QString choiceDirectory;
  int valueDotCircle;
  int index = 0;
  int valueScrolBar = 0;
  int valueRotationX = 0;
  int valueRotationY = 0;
  int valueRotationZ = 0;
  int valueMoveX = 50;
  int valueMoveY = 50;
  int valueMoveZ = 50;
  int valueZoomeLine = 0;
  int valueZoomeDot = 0;
  int valueZoomeModel = 0;
  config set = {0};
  QColor color;
  QString checkerName;
  QImage *images;
  int imagesCounter;
  QTimer *timer;
};
#endif  // MAINWINDOW_H
