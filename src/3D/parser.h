/**
 * @file parcer.c
 * @brief Этот файл содержит реализацию функций для манипулирования
 * структурами вершин, объектов и конфигураций.
 */

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_VALUE_SCALE 0.50 /**< Начальное значение для масштабирования. */

/**
 * @struct вершины
 * @brief Представляет вершину с ее свойствами.
 */
typedef struct vertex {
  int amountV; /**< Количество вершин.  */
  double *v;   /**< Массив координат вершин. */
  double minZ; /**< Минимальное значение Z-координаты. */
  double maxZ; /**< Максимальное значение Z-координаты. */
  double minY; /**< Минимальное значение Y-координаты. */
  double maxY; /**< Максимальное значение Y-координаты. */
  double minX; /**< Минимальное значение X-координаты. */
  double maxX; /**< Максимальное значение X-координаты. */
} vertex;

/**
 * @struct объект
 * @brief Представляет собой объект, состоящий из граней.
 */
typedef struct object {
  int amountF;     /**< Количество граней. */
  unsigned int *f; /**< Массив индексов вершин. */
} object;

/**
 * @struct конфигурация
 * @brief Представляет параметры конфигурации для визуализации.
 */
typedef struct config {
  int projection_type; /**< Тип проекции для визуализации. */
  int rib_type;       /**< Тип ребра для визуализации. */
  float rib_color[3]; /**< The color of ribs in RGB format. */
  float rib_thickness; /**< Толщина ребер. */
  int vertex_type; /**< Тип вершины для визуализации. */
  float vertex_color[3]; /**< Массив цветов */
  float vertex_size; /**< Размер отображаемых точек вершин. */
  float background_color[4]; /**< Массив цветов фона */
} config;

/**
 * @brief Функция производит парсинг строки
 * @param ver указатель на структуру координат вершин
 * @param obj указатель на структура соединений точек
 * @param path указатель на строку, содержащую путь до объектного файла
 * @return результат парсинга строки \n
 *     0 - OK \n
 *     1 - не открылся файл \n
 *     2 - проблема с координатами \n
 *     3 - проблема с вершинами \n
 */
int parser_vertex(vertex *ver, object *obj, const char *path);

/**
 * @brief Функия производит очистку структур
 * @param ver структура с координатами вершин
 * @param obj структура с соединением точек
 */
void free_all_struct(vertex *ver, object *obj);

/**
 * @brief Функция находит max по x, max по y, min и max по z и записывает в
 * структуру
 * @param vert указатель на структуру координат вершин
 */
void finding_minmax(vertex *vert);

/**
 * @brief Функция копирует структуры координат вершин и соединений в новые
 * структуры
 * @param new_v новая структура координат вершин
 * @param old_v старая структура координат вершин
 * @param new_o новая структура соединений
 * @param old_o старая струкрура соединений
 */
void copy_struct(vertex *new_v, vertex *old_v, object *new_o, object *old_o);

/**
 * @brief Функция записывает координаты вершины в структуру
 * @param v указатель на структуру координат вершин
 * @param str указатель на текущий символ в файле
 * @return количество координат точки \n
 *     2 - точка с координатами x, y \n
 *     3 - точка с координатами x, y, z \n
 */
int take_v(vertex *vert, char *str);

/**
 * @brief Функция записывает соединения точек в структуру
 * @param v указатель на структуру координат вершин
 * @param o указатель на структура соединений точек
 * @param str указатель на текущий символ в файле
 * @param max_f указатель на максимум по х
 * @return количество точек в текущем соединении
 */
int take_o(vertex *v, object *o, char *str, int *max_f);

/**
 * @brief Функция центрирует изображение
 * @param vert указатель на структуру координат вершин
 */
void centering(vertex *vert);

/**
 * @brief Функция вычисления масштаба
 * @param vert указатель на структуру координат вершин
 * @return масштаб
 */
double calculateScale(vertex *vert);

/**
 * @brief Функция изменения масштаба
 * @param vert указатель на структуру координат вершин
 * @param scale масштаб
 */
void setScale(vertex *vert, double scale);

/**
 * @brief Функция инициализирует минимумы и максимумы для x, y, z
 * @param vert указатель на структуру координат вершин
 */
void initMinMaxValue(vertex *vert);

/**
 * @brief Функция получает данные для вывода 3D модели
 * @param conf указатель на структуру с параметрами для вывода 3D модели
 * @param path указатель на строку, содержащую путь до объектного файла
 */
void get_config(config *c, const char *path);

/**
 * @brief Функция меняет данные для вывода 3D модели
 * @param conf указатель на структуру с параметрами для вывода 3D модели
 * @param path указатель на строку, содержащую путь до объектного файла
 */
void put_config(config *c, const char *path);
