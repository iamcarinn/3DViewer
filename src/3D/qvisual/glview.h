#ifndef GLVIEW_H
#define GLVIEW_H

#define GL_SILENCE_DEPRECATION
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QtOpenGLWidgets/QOpenGLWidget>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <Qdir>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

extern "C" {
#include "../transform.h"
}

class glView : public QOpenGLWidget {
  Q_OBJECT

 public:
  glView(QWidget *parent = nullptr);
  void translate(vertex, object);
  void IndexChanged(int);
  void DotTypeChanged(int);
  void LineTypeChanged(int);
  void freeModel();
  void setColorDot(QColor);
  void setColorLine(QColor);
  void setColorBack(QColor);
  void scrollBar(int, QString);
  void setConfig();
  QColor getColorDot();
  QColor getColorLine();
  QColor getColorBack();

 private slots:
  void resizeGL(int, int) override;
  void paintGL() override;

 private:
  QPoint mPos;
  double xRot, yRot, zRot;
  double xMove, yMove, zMove;
  double scale;
  double moveValueButton;
  double moveValueButtonTmp = 1;
  void initializeGL() override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void zeroing();
  vertex pointsGL = {0};
  object modelGL = {0};
  config set = {0};
  int proection = 0;
  int openFileFl = 0;
  int valueButtonTmp = 0;
  int lineType = 0;
  int dotType = 0;
  float lineSize = 5;
  float dotSize = 10;
  bool bMoveR;
  bool bMoveL;
  double n = -5;
  QColor colorDot;
  QColor colorLine;
  QColor colorBack;

 protected:
};

#endif  // GLVIEW_H
