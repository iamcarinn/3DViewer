#include "transform.h"

/// @brief меляет координаты точек в соответствии с входными параметрами
/// @param coordinates адрес структуры с координатами
/// @param moveX насколько мужно изменить координату по X
/// @param moveY насколько мужно изменить координату по Y
/// @param moveZ насколько мужно изменить координату по Z
/// @return 0 - ok
///         1 - структура не существует
int move_model(vertex *coordinates, double moveX, double moveY, double moveZ) {
  int vErorr = 0;
  if (coordinates != NULL) {
    for (int i = 0; i < (coordinates->amountV / 3); i++) {
      coordinates->v[(i * 3) + 0] += moveX;
      coordinates->v[(i * 3) + 1] += moveY;
      coordinates->v[(i * 3) + 2] += moveZ;
    }
  } else {
    vErorr = 1;
  }
  return vErorr;
}

/// @brief маштабирует модель, пропорционально увеличивая координаты точек
/// @param coordinates адрес структуры с координатами
/// @param scale коэфицент маштобирования модели
/// @return 0 - ok
///         1 - структура не существует
int scale_model(vertex *coordinates, double scale) {
  int vErorr = 0;
  scale++;
  if (scale < 1e-6) {
    scale = .01;
  }
  if (coordinates != NULL) {
    for (int i = 0; i < (coordinates->amountV / 3); i++) {
      coordinates->v[(i * 3) + 0] *= scale;
      coordinates->v[(i * 3) + 1] *= scale;
      coordinates->v[(i * 3) + 2] *= scale;
    }
  } else {
    vErorr = 1;
  }
  return vErorr;
}

/// @brief врашает модель вокруг оси OX, меняя координары Y Z у точек
/// @param coordinates адрес структуры с координатами
/// @param angleX угол поворота
/// @return 0 - ok
///         1 - структура не существует
int rotate_model_x(vertex *coordinates, double angleX) {
  int vErorr = 0;
  double sinus = sin(angleX);
  double cosinus = cos(angleX);
  if (coordinates != NULL) {
    for (int i = 0; i < (coordinates->amountV / 3); i++) {
      double temp_y = coordinates->v[(i * 3) + 1];
      double temp_z = coordinates->v[(i * 3) + 2];
      coordinates->v[(i * 3) + 1] = cosinus * temp_y - sinus * temp_z;
      coordinates->v[(i * 3) + 2] = sinus * temp_y + cosinus * temp_z;
    }
  } else {
    vErorr = 1;
  }
  return vErorr;
}

/// @brief врашает модель вокруг оси OY, меняя координары X Z у точек
/// @param coordinates адрес структуры с координатами
/// @param angleX угол поворота
/// @return 0 - ok
///         1 - структура не существует
int rotate_model_y(vertex *coordinates, double angleY) {
  int vErorr = 0;
  double sinus = sin(angleY);
  double cosinus = cos(angleY);
  if (coordinates != NULL) {
    for (int i = 0; i < (coordinates->amountV / 3); i++) {
      double temp_x = coordinates->v[(i * 3) + 0];
      double temp_z = coordinates->v[(i * 3) + 2];
      coordinates->v[(i * 3) + 0] = cosinus * temp_x - sinus * temp_z;
      coordinates->v[(i * 3) + 2] = sinus * temp_x + cosinus * temp_z;
    }
  } else {
    vErorr = 1;
  }
  return vErorr;
}

/// @brief врашает модель вокруг оси OZ, меняя координары X Y у точек
/// @param coordinates адрес структуры с координатами
/// @param angleX угол поворота
/// @return 0 - ok
///         1 - структура не существует
int rotate_model_z(vertex *coordinates, double angleZ) {
  int vErorr = 0;
  double sinus = sin(angleZ);
  double cosinus = cos(angleZ);
  if (coordinates != NULL) {
    for (int i = 0; i < (coordinates->amountV / 3); i++) {
      double temp_x = coordinates->v[(i * 3) + 0];
      double temp_y = coordinates->v[(i * 3) + 1];
      coordinates->v[(i * 3) + 0] = cosinus * temp_x - sinus * temp_y;
      coordinates->v[(i * 3) + 1] = sinus * temp_x + cosinus * temp_y;
    }
  } else {
    vErorr = 1;
  }
  return vErorr;
}
