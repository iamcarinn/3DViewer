/**
 * @file transform.c
 * @brief Этот файл содержит реализацию функций для изменения масштаба,
 * вычисления координат, вращения модели вокруг оси.
 */

#include "parser.h"

/**
 * @brief Функция вычисляет координаты точек в соответствии с входными
 * параметрами
 * @param coordinates адрес структуры с координатами
 * @param moveX насколько мужно изменить координату по X
 * @param moveY насколько мужно изменить координату по Y
 * @param moveZ насколько мужно изменить координату по Z
 * @return 0 - ok \n
 *         1 - структура не существует \n
 */
int move_model(vertex *coordinates, double moveX, double moveY, double moveZ);

/**
 * @brief Функция маштабирует модель, пропорционально увеличивая координаты
 * точек
 * @param coordinates адрес структуры с координатами
 * @param scale коэфицент маштобирования модели
 * @return 0 - ok \n
 *         1 - структура не существует \n
 */
int scale_model(vertex *coordinates, double scale);

/**
 * @brief Функция врашает модель вокруг оси OX, меняя координары Y Z у точек
 * @param coordinates адрес структуры с координатами
 * @param angleX угол поворота
 * @return 0 - ok \n
 *         1 - структура не существует \n
 */
int rotate_model_x(vertex *coordinates, double angleX);

/**
 * @brief Функция врашает модель вокруг оси OY, меняя координары X Z у точек
 * @param coordinates адрес структуры с координатами
 * @param angleY угол поворота
 * @return 0 - ok \n
 *         1 - структура не существует \n
 */
int rotate_model_y(vertex *coordinates, double angleY);

/**
 * @brief Функция врашает модель вокруг оси OZ, меняя координары X Y у точек
 * @param coordinates адрес структуры с координатами
 * @param angleZ угол поворота
 * @return 0 - ok \n
 *         1 - структура не существует \n
 */
int rotate_model_z(vertex *coordinates, double angleZ);
