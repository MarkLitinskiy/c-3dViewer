#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse_obj.h"
#include "transformations.h"

START_TEST(num_of_v_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  ck_assert_uint_eq(num_of_v, 8);
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(num_of_e_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  ck_assert_uint_eq(num_of_e, 36);
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(v_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  double v_static[24] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1,
                         1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static[i], vertexes[i], 1e-6);
  }
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(e_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  double e_static[72] = {0, 6, 6, 4, 4, 0, 0, 2, 2, 6, 6, 0, 0, 3, 3, 2, 2, 0,
                         0, 1, 1, 3, 3, 0, 2, 7, 7, 6, 6, 2, 2, 3, 3, 7, 7, 2,
                         4, 6, 6, 7, 7, 4, 4, 7, 7, 5, 5, 4, 0, 4, 4, 5, 5, 0,
                         0, 5, 5, 1, 1, 0, 1, 5, 5, 7, 7, 1, 1, 7, 7, 3, 3, 1};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  for (unsigned int i = 0; i < num_of_e * 2; i++) {
    ck_assert_double_eq_tol(e_static[i], facets[i], 1e-6);
  }
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(translate_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  double v_static[24] = {1, 2, 3, 1, 2, 4, 1, 3, 3, 1, 3, 4,
                         2, 2, 3, 2, 2, 4, 2, 3, 3, 2, 3, 4};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  translate(1, 2, 3, vertexes, num_of_v * 3);
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static[i], vertexes[i], 1e-6);
  }
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(translateX_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  double v_static[24] = {1, 2, 3, 1, 2, 4, 1, 3, 3, 1, 3, 4,
                         2, 2, 3, 2, 2, 4, 2, 3, 3, 2, 3, 4};
  double x_prev = 0;
  double y_prev = 0;
  double z_prev = 0;
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  translateX(&x_prev, 1, &y_prev, 2, &z_prev, 3, vertexes, num_of_v * 3);
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static[i], vertexes[i], 1e-6);
  }
  translateX(&x_prev, 0, &y_prev, 0, &z_prev, 0, vertexes, num_of_v * 3);
  double v_static_2[24] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1,
                           1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1};
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static_2[i], vertexes[i], 1e-6);
  }
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(rotate_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  double v_static[24] = {0.000000,  0.000000,  0.000000,  0.739199,  0.280330,
                         0.612372,  -0.573223, 0.739199,  0.353553,  0.165976,
                         1.019529,  0.965926,  0.353553,  0.612372,  -0.707107,
                         1.092752,  0.892703,  -0.094734, -0.219670, 1.351571,
                         -0.353553, 0.519529,  1.631901,  0.258819};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  rotate(30, 45, 60, vertexes, num_of_v * 3);
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static[i], vertexes[i], 1e-6);
  }
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(rotateX_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  double x_prev = 0;
  double y_prev = 0;
  double z_prev = 0;
  double v_static[24] = {0.000000,  0.000000,  0.000000,  0.739199,  0.280330,
                         0.612372,  -0.573223, 0.739199,  0.353553,  0.165976,
                         1.019529,  0.965926,  0.353553,  0.612372,  -0.707107,
                         1.092752,  0.892703,  -0.094734, -0.219670, 1.351571,
                         -0.353553, 0.519529,  1.631901,  0.258819};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  rotateX(&x_prev, 30, &y_prev, 45, &z_prev, 60, vertexes, num_of_v * 3);
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static[i], vertexes[i], 1e-6);
  }
  rotateX(&x_prev, 0, &y_prev, 0, &z_prev, 0, vertexes, num_of_v * 3);
  double v_static_2[24] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1,
                           1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1};
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static_2[i], vertexes[i], 1e-6);
  }
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(scale_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  double v_static[24] = {0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
                         1.900000, 0.000000, 1.500000, 0.000000, 0.000000,
                         1.500000, 1.900000, 1.200000, 0.000000, 0.000000,
                         1.200000, 0.000000, 1.900000, 1.200000, 1.500000,
                         0.000000, 1.200000, 1.500000, 1.900000};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  scale(1.2, 1.5, 1.9, vertexes, num_of_v * 3);
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static[i], vertexes[i], 1e-6);
  }
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(scaleX_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  double prev_scale = 1;
  double new_scale = 1.75;
  double new_scale_2 = 1;
  double v_static[24] = {0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
                         1.750000, 0.000000, 1.750000, 0.000000, 0.000000,
                         1.750000, 1.750000, 1.750000, 0.000000, 0.000000,
                         1.750000, 0.000000, 1.750000, 1.750000, 1.750000,
                         0.000000, 1.750000, 1.750000, 1.750000};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  scaleX(&prev_scale, &new_scale, vertexes, num_of_v * 3);
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static[i], vertexes[i], 1e-6);
  }
  double v_static_2[24] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1,
                           1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1};
  scaleX(&prev_scale, &new_scale_2, vertexes, num_of_v * 3);
  for (unsigned int i = 0; i < num_of_v * 3; i++) {
    ck_assert_double_eq_tol(v_static_2[i], vertexes[i], 1e-6);
  }
  free(facets);
  free(vertexes);
}
END_TEST

START_TEST(coord_test) {
  char *filename = "3dModels/cube.obj";
  unsigned int num_of_v = 0;
  unsigned int num_of_e = 0;
  double *vertexes = NULL;
  int *facets = NULL;
  coordinates_t coords = {0, 0, 0, 0, 0, 0};
  parse_obj(filename, &num_of_v, &num_of_e, &vertexes, &facets, &coords);
  ck_assert_double_eq_tol(-1, coords.Zmin, 1e-6);
  ck_assert_double_eq_tol(1, coords.Zmax, 1e-6);
  ck_assert_double_eq_tol(-1, coords.Ymin, 1e-6);
  ck_assert_double_eq_tol(1, coords.Ymax, 1e-6);
  ck_assert_double_eq_tol(-1, coords.Xmin, 1e-6);
  ck_assert_double_eq_tol(1, coords.Xmax, 1e-6);
  free(facets);
  free(vertexes);
}
END_TEST

Suite *f_example_suite_create() {
  Suite *s1 = suite_create("Test_3d");
  TCase *core_test = tcase_create("All");
  suite_add_tcase(s1, core_test);
  tcase_add_test(core_test, num_of_v_test);
  tcase_add_test(core_test, num_of_e_test);
  tcase_add_test(core_test, v_test);
  tcase_add_test(core_test, e_test);
  tcase_add_test(core_test, coord_test);
  tcase_add_test(core_test, translate_test);
  tcase_add_test(core_test, rotate_test);
  tcase_add_test(core_test, scale_test);
  tcase_add_test(core_test, translateX_test);
  tcase_add_test(core_test, rotateX_test);
  tcase_add_test(core_test, scaleX_test);
  return s1;
}

int main() {
  Suite *s1 = f_example_suite_create();
  // Runner
  SRunner *runner = srunner_create(s1);
  int number_failed;
  srunner_run_all(runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return number_failed == 0 ? 0 : 1;
}
