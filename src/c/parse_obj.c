#include "parse_obj.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_obj(char *filename, unsigned int *num_of_vertexes,
              unsigned int *num_of_edges, double **vertexes, int **facets,
              coordinates_t *coordinates) {
  int error = 0; /* TODO define error codes (file not found, ... */
  *num_of_edges = 0;
  *num_of_vertexes = 0;
  coordinates->Xmax = 1;
  coordinates->Xmin = -1;
  coordinates->Ymax = 1;
  coordinates->Ymin = -1;
  coordinates->Zmax = 1;
  coordinates->Zmin = -1;
  FILE *fp = fopen(filename, "r");
  if (fp != NULL) {
    count_vertexes_and_facets(fp, num_of_vertexes, num_of_edges);
    fseek(fp, 0, SEEK_SET);
    *vertexes = calloc((*num_of_vertexes * 3), sizeof(double));
    *facets =
        calloc(*num_of_edges * 2, sizeof(int)); /* TODO check for errors? */
    read_data(fp, *vertexes, *facets, coordinates);
    fclose(fp);
  } else {
    fprintf(stderr, "can't open file\n");
  }
  return error;
}

int read_vertex(double *vertexes, char *str, int *v_ind,
                coordinates_t *coordinates) {
  int error = 0; /* TODO define error codes? */
  double tmp1 = 0, tmp2 = 0, tmp3 = 0;
  int num_of_digits_read = sscanf(str, "v %lf %lf %lf", &tmp1, &tmp2, &tmp3);
  if (num_of_digits_read != 3) {
    error = PARSE_ERROR;
  } else {
    vertexes[*v_ind] = tmp1;
    vertexes[*v_ind + 1] = tmp2;
    vertexes[*v_ind + 2] = tmp3;
    if (tmp1 < coordinates->Xmin) {
      coordinates->Xmin = tmp1;
    }
    if (tmp1 > coordinates->Xmax) {
      coordinates->Xmax = tmp1;
    }
    if (tmp2 < coordinates->Ymin) {
      coordinates->Ymin = tmp2;
    }
    if (tmp2 > coordinates->Ymax) {
      coordinates->Ymax = tmp2;
    }
    if (tmp3 < coordinates->Zmin) {
      coordinates->Zmin = tmp3;
    }
    if (tmp3 > coordinates->Zmax) {
      coordinates->Zmax = tmp3;
    }
    *v_ind += 3;
  }
  return error;
}

int count_vertexes_and_facets(FILE *fp, unsigned int *num_of_vertexes,
                              unsigned int *num_of_edges) {
  int err = OK;
  size_t len = 0;
  ssize_t num_of_chars = 0;
  char *str = NULL;
  double tmp = 0;
  int i = 0;
  num_of_chars = getline(&str, &len, fp);
  while (num_of_chars != -1) {
    if ((str[0] == 'v') && (str[1] != 'n')) {
      *num_of_vertexes += 1;
    } else if ((str[0] == 'f') && (str[1] == ' ')) {
      i = 0;
      sscanf(str, "f %lf", &tmp);
      while (str[i] != '\n') {
        if ((str[i] == ' ') && (is_digit(str[i + 1]) == 1)) {
          *num_of_edges += 1;
        }
        i++;
      }
      /* *num_of_edges += 1; /\* f 1 2 3 -> 1 2 3 1 *\/ */
    } else {
      /* parse error? */
    }
    num_of_chars = getline(&str, &len, fp);
  }
  /* f 1 2 3 -> 1 2 2 3 3 1 */
  free(str);
  return err;
}

int read_data(FILE *fp, double *vertexes, int *facets,
              coordinates_t *coordinates) {
  int error = OK;
  int v_ind = 0;
  int f_ind = 0;
  size_t len = 0;
  ssize_t num_of_chars = 0;
  char *str = NULL;

  num_of_chars = getline(&str, &len, fp);
  while (num_of_chars != -1) {
    if ((str[0] == 'v') && (str[1] != 'n')) {
      read_vertex(vertexes, str, &v_ind, coordinates);
    } else if (str[0] == 'f') {
      read_facet(facets, str, &f_ind);
    }
    num_of_chars = getline(&str, &len, fp);
  }
  free(str);
  return error;
}

int read_facet(int *facets, char *str, int *f_ind) {
  int error = OK;
  int first_v = 0;
  int prev_v = 0;
  int curr_v = 0;
  size_t len = strlen(str);
  size_t i = 1;
  sscanf(str, "f %d", &prev_v);
  prev_v -= 1;
  first_v = prev_v;
  while ((i + 1 < len) && (is_digit(str[i + 1]) != 1)) {
    i++;
  }
  i++;
  while (i < strlen(str)) {
    if ((str[i] == ' ') && (is_digit(str[i + 1]) == 1)) {
      sscanf(str + i, "%d", &curr_v);
      curr_v -= 1;
      facets[*f_ind] = prev_v;
      *f_ind += 1;
      facets[*f_ind] = curr_v;
      *f_ind += 1;
      prev_v = curr_v;
    }
    i++;
  }
  facets[*f_ind] = prev_v;
  *f_ind += 1;
  facets[*f_ind] = first_v;
  *f_ind += 1;
  return error;
}

int is_digit(char ch) { return ((ch >= 48) && (ch <= 57)); }
