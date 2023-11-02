#include "glview.h"

glView::glView(QWidget* parent) : QOpenGLWidget(parent) {
  QString dirPath = QDir::home().absolutePath() + "/Desktop/config.txt";
  get_config(&set, dirPath.toStdString().c_str());
  colorBack.setRgbF(set.background_color[0], set.background_color[1],
                    set.background_color[2], set.background_color[3]);
  colorDot.setRgbF(set.vertex_color[0], set.vertex_color[1],
                   set.vertex_color[2]);
  colorLine.setRgbF(set.rib_color[0], set.rib_color[1], set.rib_color[2]);
  lineType = set.rib_type;
  dotType = set.vertex_type;
  lineSize = set.rib_thickness;
  dotSize = set.vertex_size;
  proection = set.projection_type;
}

void glView::initializeGL() { glEnable(GL_DEPTH_TEST); }

void glView::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void glView::paintGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glClearColor(colorBack.redF(), colorBack.greenF(), colorBack.blueF(),
               colorBack.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glVertexPointer(3, GL_DOUBLE, 0, pointsGL.v);
  glEnableClientState(GL_VERTEX_ARRAY);
  if (openFileFl) {
    if (proection == 0) {
      glOrtho(-(1), 1, -(1), 1, 1, 5);
      glTranslatef(0, 0, -2);
    } else {
      glFrustum(-(2), 2, -(2), 2, 1, 5);
      glTranslatef(0, 0, -3);
    }
    rotate_model_x(&pointsGL, xRot);
    rotate_model_y(&pointsGL, yRot);
    rotate_model_z(&pointsGL, zRot);
    move_model(&pointsGL, xMove, -yMove, zMove);
    scale_model(&pointsGL, scale);
    zeroing();
    glPointSize(dotSize * 0.5);
    glColor3f(colorDot.redF(), colorDot.greenF(), colorDot.blueF());
    if (dotType == 0) {
      glEnable(GL_POINT_SMOOTH);
      glDrawArrays(GL_POINTS, 0, pointsGL.amountV / 3);
      glDisable(GL_POINT_SMOOTH);
    } else if (dotType == 1) {
      glDrawArrays(GL_POINTS, 0, pointsGL.amountV / 3);
    }
    glColor3f(colorLine.redF(), colorLine.greenF(), colorLine.blueF());
    glLineWidth(lineSize * 0.3);
    if (lineType == 0) {
      glDisable(GL_LINE_STIPPLE);
      glDrawElements(GL_LINES, (modelGL.amountF), GL_UNSIGNED_INT, modelGL.f);
    } else if (lineType == 1) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00F0);
      glDrawElements(GL_LINES, (modelGL.amountF), GL_UNSIGNED_INT, modelGL.f);
    }
    glDisable(GL_LINE_STIPPLE);
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}

void glView::zeroing() {
  xMove = 0;
  yMove = 0;
  zMove = 0;
  yRot = 0;
  xRot = 0;
  zRot = 0;
  scale = 0;
}

void glView::mousePressEvent(QMouseEvent* event) {
  mPos = event->pos();
  if (event->button() == Qt::LeftButton) {
    bMoveL = true;
  } else if (event->button() == Qt::RightButton) {
    bMoveR = true;
  }
}

void glView::mouseMoveEvent(QMouseEvent* event) {
  if (event->buttons() & Qt::LeftButton) {
    xRot = .01 * (event->pos().y() - mPos.y());
    yRot = .01 * (event->pos().x() - mPos.x());
  }
  if (event->buttons() & Qt::RightButton) {
    xMove = .005 * (event->pos().x() - mPos.x());
    yMove = .005 * (event->pos().y() - mPos.y());
  }
  mPos = event->pos();
  update();
}

void glView::scrollBar(int valueButton, QString check) {
  if (check == "radioButtonRotationX") {
    xRot = .0635 * (valueButton - valueButtonTmp);
    valueButtonTmp = valueButton;
    qDebug() << "x" << xRot;
  } else if (check == "radioButtonRotationY") {
    yRot = .0635 * (valueButton - valueButtonTmp);
    valueButtonTmp = valueButton;
    qDebug() << "y" << yRot;
  } else if (check == "radioButtonRotationZ") {
    zRot = .0635 * (valueButton - valueButtonTmp);
    valueButtonTmp = valueButton;
    qDebug() << "z" << zRot;
  } else if (check == "radioButtonMoveX") {
    moveValueButton = valueButton / 50.0;
    xMove = 3.5 * (moveValueButton - moveValueButtonTmp);
    moveValueButtonTmp = moveValueButton;
  } else if (check == "radioButtonMoveY") {
    moveValueButton = valueButton / 50.0;
    yMove = 3.5 * (moveValueButton - moveValueButtonTmp);
    moveValueButtonTmp = moveValueButton;
  } else if (check == "radioButtonMoveZ") {
    moveValueButton = valueButton / 50.0;
    zMove = 3.5 * (moveValueButton - moveValueButtonTmp);
    moveValueButtonTmp = moveValueButton;
  } else if (check == "radioButtonZoomeLine") {
    lineSize = (float)valueButton;
    valueButtonTmp = valueButton;
  } else if (check == "radioButtonZoomeDot") {
    dotSize = (float)valueButton;
    moveValueButtonTmp = moveValueButton;
  } else if (check == "radioButtonZoomeModel") {
    moveValueButton = valueButton;
    scale = (moveValueButton - moveValueButtonTmp) * 0.02;
    moveValueButtonTmp = moveValueButton;
  }
  update();
}

void glView::wheelEvent(QWheelEvent* event) {
  QPoint circle = event->pixelDelta();
  if ((event->buttons() & Qt::LeftButton) && bMoveL) {
    zRot = ((double)circle.y() * .01);
  } else if ((event->buttons() & Qt::RightButton) && bMoveR) {
    zMove = ((double)circle.y() * .005);
  } else if (!bMoveL && !bMoveR) {
    scale = ((double)circle.y() * .01);
  }
  update();
}

void glView::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton && bMoveL) {
    bMoveL = false;
  }
  if (event->button() == Qt::RightButton && bMoveR) {
    bMoveR = false;
  }
}

void glView::translate(vertex p, object m) {
  free_all_struct(&pointsGL, &modelGL);
  copy_struct(&pointsGL, &p, &modelGL, &m);
  openFileFl = 1;
  update();
}

void glView::IndexChanged(int proectionin) {
  proection = proectionin;
  paintGL();
  update();
}

void glView::DotTypeChanged(int index) {
  dotType = index;
  paintGL();
  update();
}

void glView::LineTypeChanged(int index) {
  lineType = index;
  qDebug() << "2" << lineType;
  paintGL();
  update();
}

void glView::freeModel() { free_all_struct(&pointsGL, &modelGL); }

void glView::setColorDot(QColor color) { colorDot = color; }

void glView::setColorLine(QColor color) { colorLine = color; }

void glView::setColorBack(QColor color) { colorBack = color; }

QColor glView::getColorDot() { return this->colorDot; }
QColor glView::getColorLine() { return this->colorLine; }
QColor glView::getColorBack() { return this->colorBack; }

void glView::setConfig() {
  set.background_color[0] = colorBack.redF();
  set.background_color[1] = colorBack.greenF();
  set.background_color[2] = colorBack.blueF();
  set.background_color[3] = colorBack.alphaF();
  set.vertex_color[0] = colorDot.redF();
  set.vertex_color[1] = colorDot.greenF();
  set.vertex_color[2] = colorDot.blueF();
  set.rib_color[0] = colorLine.redF();
  set.rib_color[1] = colorLine.greenF();
  set.rib_color[2] = colorLine.blueF();
  set.projection_type = proection;
  set.rib_thickness = lineSize;
  set.vertex_size = dotSize;
  set.vertex_type = dotType;
  set.rib_type = lineType;
  QString dirPath = QDir::home().absolutePath() + "/Desktop/config.txt";
  put_config(&set, dirPath.toStdString().c_str());
}
