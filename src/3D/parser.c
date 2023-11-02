#include "parser.h"

/// @brief Функия производит очистку структур
/// @param v структура с координатами вершин
/// @param o структура с соединением точек
void free_all_struct(vertex *v, object *o) {
  if (o && o->f) {
    free(o->f);
  }
  if (v && v->v) {
    free(v->v);
  }
}

/// @brief Функция записывает соединения точек в структуру
/// @param o указатель на структура соединений точек
/// @param str указатель на текущий символ в файле
/// @param max_f указатель на максимум по х
/// @return количество точек в текущем соединении
int take_o(vertex *v, object *o, char *str, int *max_f) {
  int res = 0;
  size_t i = 0;
  char *tmp = strtok(str, " \r\n");
  while (tmp != NULL && !res) {
    if (!(o->f[o->amountF + i] % 500)) {
      o->f = (unsigned int *)realloc(
          o->f, (o->amountF + i + 500) * sizeof(unsigned int));
    }
    int x = atoi(tmp) - 1;
    if (x == -1) res = 4;
    if (x >= 0) {
      o->f[i + o->amountF] = x;
      *max_f = *max_f < x ? x : *max_f;
      tmp = strtok(NULL, " \r\n");
      if (i) {
        o->f[++i + o->amountF] = x;
      }
      i++;
    } else {
      x -= -(v->amountV / 3) - 1;
      if (x < 0) res = 3;
      o->f[i + o->amountF] = x;
      *max_f = *max_f < x ? x : *max_f;
      tmp = strtok(NULL, " \r\n");
      if (i) {
        o->f[++i + o->amountF] = x;
      }
      i++;
    }
  }
  if (i != 1) {
    o->f[i + o->amountF] = o->f[o->amountF];
    o->amountF += i + 1;
  }
  return res;
}

/// @brief Функция производит парсинг строки
/// @param ver указатель на структуру координат вершин
/// @param obj указатель на структура соединений точек
/// @param path указатель на строку, содержащую путь до объектного файла
/// @return результат парсинга строки \n
///     0 - OK \n
///     1 - не открылся файл \n
///     2 - проблема с координатами \n
///     3 - проблема с вершинами \n
int parser_vertex(vertex *ver, object *obj, const char *path) {
  int res = 0, max_f = 0;
  char *s = NULL;
  size_t len = 0;
  ver->amountV = 0;
  obj->amountF = 0;
  FILE *f;
  f = fopen(path, "r");
  if (f) {
    ver->v = (double *)calloc(300, sizeof(double));
    obj->f = (unsigned int *)calloc(500, sizeof(unsigned int));
    initMinMaxValue(ver);
    while ((getline(&s, &len, f) != -1) && !res) {
      if (s[0] == 'v' && s[1] == ' ') {
        if (!(ver->amountV % 300)) {
          ver->v =
              (double *)realloc(ver->v, (300 + ver->amountV) * sizeof(double));
        }
        res = take_v(ver, &s[1]);
        finding_minmax(ver);
        ver->amountV += 3;
      } else if (s[0] == 'f' && s[1] == ' ') {
        res = take_o(ver, obj, &s[1], &max_f);
      }
    }
    if (s) {
      free(s);
    }
    fclose(f);
  } else {
    res = 1;
  }
  res = max_f > (ver->amountV / 3) ? 3 : res;
  if (!res) {
    centering(ver);
    double scale = calculateScale(ver);
    setScale(ver, scale);
  }
  return res;
}

/// @brief Функция записывает координаты вершины в структуру
/// @param v указатель на структуру координат вершин
/// @param str указатель на текущий символ в файле
/// @return количество координат точки \n
///     2 - точка с координатами x, y \n
///     3 - точка с координатами x, y, z \n
int take_v(vertex *v, char *str) {
  int res = 0;
  char *end = str;
  size_t i = 0;
  while (*str) {
    v->v[v->amountV + i] = strtod(str, &end);
    str = end;
    i++;
    while (!isdigit(*str) && *str && *str != '-') str++;
  }
  res = i == 3 ? res : 2;
  return res;
}

/// @brief Функция находит max по x, max по y, min и max по z и записывает в
/// структуру
/// @param ver указатель на структуру координат вершин
void finding_minmax(vertex *ver) {
  ver->minX = fmin(ver->minX, ver->v[ver->amountV]);
  ver->minY = fmin(ver->minY, ver->v[ver->amountV + 1]);
  ver->minZ = fmin(ver->minZ, ver->v[ver->amountV + 2]);
  ver->maxX = fmax(ver->maxX, ver->v[ver->amountV]);
  ver->maxY = fmax(ver->maxY, ver->v[ver->amountV + 1]);
  ver->maxZ = fmax(ver->maxZ, ver->v[ver->amountV + 2]);
}

/// @brief Функция копирует структуры координат вершин и соединений в новые
/// структуры
/// @param new_v новая структура координат вершин
/// @param old_v старая структура координат вершин
/// @param new_o новая структура соединений
/// @param old_o старая струкрура соединений
void copy_struct(vertex *new_v, vertex *old_v, object *new_o, object *old_o) {
  new_v->v = (double *)calloc(old_v->amountV, sizeof(double));
  new_o->f = (unsigned int *)calloc(old_o->amountF, sizeof(unsigned int));
  new_v->amountV = old_v->amountV;
  new_o->amountF = old_o->amountF;
  new_v->maxZ = old_v->maxZ;
  new_v->minZ = old_v->minZ;
  new_v->maxY = old_v->maxY;
  new_v->minY = old_v->minY;
  new_v->maxX = old_v->maxX;
  new_v->minX = old_v->minX;
  for (int i = 0; i < old_v->amountV; i++) {
    new_v->v[i] = old_v->v[i];
  }
  for (int i = 0; i < old_o->amountF; i++) {
    new_o->f[i] = old_o->f[i];
  }
}

/// @brief Функция получает данные для вывода 3D модели
/// @param c указатель на структуру с параметрами для вывода 3D модели
void get_config(config *c, const char *path) {
  FILE *f;
  f = fopen(path, "r");
  if (f) {
    fscanf(f, "%d %d %f %f %f %f %d %f %f %f %f %f %f %f %f",
           &c->projection_type, &c->rib_type, &c->rib_color[0],
           &c->rib_color[1], &c->rib_color[2], &c->rib_thickness,
           &c->vertex_type, &c->vertex_color[0], &c->vertex_color[1],
           &c->vertex_color[2], &c->vertex_size, &c->background_color[0],
           &c->background_color[1], &c->background_color[2],
           &c->background_color[3]);
    fclose(f);
  } else {
    c->projection_type = 0, c->rib_type = 0, c->rib_color[0] = 0.031739,
    c->rib_color[1] = 0.120958, c->rib_color[2] = 0.642680,
    c->rib_thickness = 12, c->vertex_type = 0, c->vertex_color[0] = 0.915587,
    c->vertex_color[1] = 0.200717, c->vertex_color[2] = 0.137087,
    c->vertex_size = 21.000000, c->background_color[0] = 0.807492,
    c->background_color[1] = 0.992538, c->background_color[2] = 0.390356,
    c->background_color[3] = 0;
  }
}

/// @brief Функция меняет данные для вывода 3D модели
/// @param c указатель на структуру с параметрами для вывода 3D модели
void put_config(config *c, const char *path) {
  FILE *f;
  f = fopen(path, "w+");
  if (f) {
    fprintf(f, "%d %d %f %f %f %f %d %f %f %f %f %f %f %f %f",
            c->projection_type, c->rib_type, c->rib_color[0], c->rib_color[1],
            c->rib_color[2], c->rib_thickness, c->vertex_type,
            c->vertex_color[0], c->vertex_color[1], c->vertex_color[2],
            c->vertex_size, c->background_color[0], c->background_color[1],
            c->background_color[2], c->background_color[3]);
    fclose(f);
  }
}

/// @brief
/// @param vert
void centering(vertex *vert) {
  double centerX = vert->minX + (vert->maxX - vert->minX) / 2;
  double centerY = vert->minY + (vert->maxY - vert->minY) / 2;
  double centerZ = vert->minZ + (vert->maxZ - vert->minZ) / 2;
  for (int i = 0; i < vert->amountV; ++i) {
    switch (i % 3) {
      case 0:
        vert->v[i] -= centerX;
        break;
      case 1:
        vert->v[i] -= centerY;
        break;
      case 2:
        vert->v[i] -= centerZ;
        break;
    }
  }
}

/// @brief
/// @param vert
/// @return
double calculateScale(vertex *vert) {
  double x = vert->maxX - vert->minX;
  double y = vert->maxY - vert->minY;
  double z = vert->maxZ - vert->minZ;
  double max = fmax(fmax(x, y), z);
  return (START_VALUE_SCALE) / max;
}

/// @brief
/// @param vert
/// @param scale
void setScale(vertex *vert, double scale) {
  for (int i = 0; i < vert->amountV; ++i) vert->v[i] *= scale;
}

/// @brief
/// @param vert
void initMinMaxValue(vertex *vert) {
  vert->minX = DBL_MAX;
  vert->minY = DBL_MAX;
  vert->minZ = DBL_MAX;
  vert->maxX = -DBL_MAX;
  vert->maxY = -DBL_MAX;
  vert->maxZ = -DBL_MAX;
}
