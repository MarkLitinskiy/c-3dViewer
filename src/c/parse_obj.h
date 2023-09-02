#ifndef PARSE_OBJ_H
#define PARSE_OBJ_H

#define OK 0
#define PARSE_ERROR 1

#include <math.h>
#include <stdio.h>

typedef struct coordinates {
  double Xmin;
  double Xmax;
  double Ymin;
  double Ymax;
  double Zmin;
  double Zmax;
} coordinates_t;

int parse_obj(char *filename, unsigned int *num_of_vertexes,
              unsigned int *num_of_facets, double **vertexes, int **facets,
              coordinates_t *coordinates);
int count_vertexes_and_facets(FILE *fp, unsigned int *num_of_vertexes,
                              unsigned int *num_of_facets);
int read_data(FILE *fp, double *vertexes, int *facets,
              coordinates_t *coordinates);
int read_vertex(double *vertexes, char *str, int *v_ind,
                coordinates_t *coordinates);
int read_facet(int *facets, char *str, int *f_ind);
int read_next_facet(char *str, int *ind);
int is_digit(char ch);

#endif
