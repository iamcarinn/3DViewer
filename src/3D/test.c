#include <check.h>

#include "transform.h"

// валидные данные
START_TEST(s21_3Dviever_1) {
  vertex v;
  object o;
  int res = parser_vertex(&v, &o, "test.obj");
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq(v.amountV, 24);
  ck_assert_double_eq_tol(v.v[0], -0.194444, 1e-6);
  ck_assert_double_eq_tol(v.v[1], -0.111111, 1e-6);
  ck_assert_double_eq_tol(v.v[2], 0.0833333, 1e-6);

  free_all_struct(&v, &o);
}
END_TEST

// точки с координатами xy
START_TEST(s21_3Dviever_2) {
  vertex v;
  object o;
  int res = parser_vertex(&v, &o, "test_xy.obj");
  ck_assert_int_eq(res, 2);

  free_all_struct(&v, &o);
}
END_TEST

// // подается число большее, чем кол-во вершин
START_TEST(s21_3Dviever_3) {
  vertex v;
  object o;
  int res = parser_vertex(&v, &o, "test_minf.obj");
  ck_assert_int_eq(res, 3);

  free_all_struct(&v, &o);
}
END_TEST

// // подается отрицательный номер вершины
START_TEST(s21_3Dviever_4) {
  vertex v;
  object o;
  int res = parser_vertex(&v, &o, "test_negf.obj");
  ck_assert_int_eq(res, 0);
  free_all_struct(&v, &o);
}
END_TEST

// // подается нулевая вершина
START_TEST(s21_3Dviever_5) {
  vertex v;
  object o;
  int res = parser_vertex(&v, &o, "test_zero.obj");
  ck_assert_int_eq(res, 4);

  free_all_struct(&v, &o);
}
END_TEST

// // несуществующий файл
START_TEST(s21_3Dviever_6) {
  vertex v;
  object o;
  int res = parser_vertex(&v, &o, "test_no.obj");
  ck_assert_int_eq(res, 1);

  // free_all_struct(&v, &o);
}
END_TEST

START_TEST(s21_3Dviever_7) {
  config c;
  get_config(&c, "config.txt");
  ck_assert_int_eq(c.projection_type, 0);
  ck_assert_int_eq(c.rib_type, 0);
  ck_assert_double_eq_tol(c.rib_color[0], 0.031739, 1e-6);
  ck_assert_double_eq_tol(c.rib_color[1], 0.120958, 1e-6);
  ck_assert_double_eq_tol(c.rib_color[2], 0.642680, 1e-6);
  ck_assert_double_eq_tol(c.rib_thickness, 12, 1e-6);
  ck_assert_int_eq(c.vertex_type, 0);
  ck_assert_double_eq_tol(c.vertex_color[0], 0.915587, 1e-6);
  ck_assert_double_eq_tol(c.vertex_color[1], 0.200717, 1e-6);
  ck_assert_double_eq_tol(c.vertex_color[2], 0.137087, 1e-6);
  ck_assert_double_eq_tol(c.vertex_size, 21, 1e-6);

  ck_assert_double_eq_tol(c.background_color[0], 0.807492, 1e-6);
  ck_assert_double_eq_tol(c.background_color[1], 0.992538, 1e-6);
  ck_assert_double_eq_tol(c.background_color[2], 0.390356, 1e-6);
  ck_assert_double_eq_tol(c.background_color[3], 0, 1e-6);
}
END_TEST

START_TEST(s21_3Dviever_8) {
  vertex v;
  object o;
  parser_vertex(&v, &o, "test.obj");
  rotate_model_x(&v, 0.5);
  ck_assert_double_eq_tol(v.v[0], -0.194444, 1e-6);
  ck_assert_double_eq_tol(v.v[1], -0.137461, 1e-6);
  ck_assert_double_eq_tol(v.v[2], 0.0198624, 1e-6);

  free_all_struct(&v, &o);
}

START_TEST(s21_3Dviever_9) {
  vertex v;
  object o;
  parser_vertex(&v, &o, "test.obj");
  rotate_model_y(&v, 0.5);
  ck_assert_double_eq_tol(v.v[0], -0.210593, 1e-6);
  ck_assert_double_eq_tol(v.v[1], -0.111111, 1e-6);
  ck_assert_double_eq_tol(v.v[2], -0.0200898, 1e-6);

  free_all_struct(&v, &o);
}

START_TEST(s21_3Dviever_10) {
  vertex v;
  object o;
  parser_vertex(&v, &o, "test.obj");
  rotate_model_z(&v, 0.5);
  ck_assert_double_eq_tol(v.v[0], -0.117372, 1e-6);
  ck_assert_double_eq_tol(v.v[1], -0.190731, 1e-6);
  ck_assert_double_eq_tol(v.v[2], 0.0833333, 1e-6);

  free_all_struct(&v, &o);
}

START_TEST(s21_3Dviever_11) {
  vertex v;
  object o;
  parser_vertex(&v, &o, "test.obj");
  scale_model(&v, 0.5);
  ck_assert_double_eq_tol(v.v[0], -0.291667, 1e-6);
  ck_assert_double_eq_tol(v.v[1], -0.166667, 1e-6);
  ck_assert_double_eq_tol(v.v[2], 0.125, 1e-6);

  free_all_struct(&v, &o);
}

START_TEST(s21_3Dviever_12) {
  vertex v;
  object o;
  parser_vertex(&v, &o, "test.obj");
  move_model(&v, 0.5, 0.5, 0.5);
  ck_assert_double_eq_tol(v.v[0], 0.305556, 1e-6);
  ck_assert_double_eq_tol(v.v[1], 0.388889, 1e-6);
  ck_assert_double_eq_tol(v.v[2], 0.583333, 1e-6);

  free_all_struct(&v, &o);
}

START_TEST(s21_3Dviever_13) {
  config c = {0};
  put_config(&c, "config.txt");
  get_config(&c, "config.txt");
  ck_assert_int_eq(c.projection_type, 0);
  ck_assert_int_eq(c.rib_type, 0);
  ck_assert_double_eq_tol(c.rib_color[0], 0, 1e-6);
  ck_assert_double_eq_tol(c.rib_color[1], 0, 1e-6);
  ck_assert_double_eq_tol(c.rib_color[2], 0, 1e-6);
  ck_assert_double_eq_tol(c.rib_thickness, 0, 1e-6);
  ck_assert_int_eq(c.vertex_type, 0);
  ck_assert_double_eq_tol(c.vertex_color[0], 0, 1e-6);
  ck_assert_double_eq_tol(c.vertex_color[1], 0, 1e-6);
  ck_assert_double_eq_tol(c.vertex_color[2], 0, 1e-6);
  ck_assert_double_eq_tol(c.vertex_size, 0, 1e-6);

  ck_assert_double_eq_tol(c.background_color[0], 0, 1e-6);
  ck_assert_double_eq_tol(c.background_color[1], 0, 1e-6);
  ck_assert_double_eq_tol(c.background_color[2], 0, 1e-6);
  ck_assert_double_eq_tol(c.background_color[3], 0, 1e-6);
}

START_TEST(s21_3Dviever_14) {
  vertex v;
  object o;
  vertex nv;
  object no;
  int res = parser_vertex(&v, &o, "test.obj");
  ck_assert_int_eq(res, 0);
  copy_struct(&nv, &v, &no, &o);
  ck_assert_double_eq(v.amountV, nv.amountV);
  ck_assert_double_eq_tol(v.v[0], nv.v[0], 1e-6);
  ck_assert_double_eq_tol(v.v[1], nv.v[1], 1e-6);
  ck_assert_double_eq_tol(v.v[2], nv.v[2], 1e-6);

  free_all_struct(&v, &o);
  free_all_struct(&nv, &no);
}
END_TEST

// START_TEST(s21_3Dviever_13) {
//   vertex v = NULL;
//   object o;
//   int res = move_model(&v, 0.5, 0.5, 0.5);
//   ck_assert_int_eq(res, 1);
//   res = scale_model(&v, 0.5);
//   ck_assert_int_eq(res, 1);
//   res = rotate_model_x(&v, 0.5);
//   ck_assert_int_eq(res, 1);
//   res = rotate_model_y(&v, 0.5);
//   ck_assert_int_eq(res, 1);
//   res = rotate_model_z(&v, 0.5);
//   ck_assert_int_eq(res, 1);

//   free_all_struct(&v, &o);
// }

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, s21_3Dviever_1);
  tcase_add_test(tc1_1, s21_3Dviever_2);
  tcase_add_test(tc1_1, s21_3Dviever_3);
  tcase_add_test(tc1_1, s21_3Dviever_4);
  tcase_add_test(tc1_1, s21_3Dviever_5);
  tcase_add_test(tc1_1, s21_3Dviever_6);
  tcase_add_test(tc1_1, s21_3Dviever_7);
  tcase_add_test(tc1_1, s21_3Dviever_8);
  tcase_add_test(tc1_1, s21_3Dviever_9);
  tcase_add_test(tc1_1, s21_3Dviever_10);
  tcase_add_test(tc1_1, s21_3Dviever_11);
  tcase_add_test(tc1_1, s21_3Dviever_12);
  tcase_add_test(tc1_1, s21_3Dviever_13);
  tcase_add_test(tc1_1, s21_3Dviever_14);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
